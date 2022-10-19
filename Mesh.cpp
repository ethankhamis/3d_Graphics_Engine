#include "Mesh.h"
#include "imgui/imgui.h"

Mesh::Mesh(Graphics& gfx, vector<unique_ptr<Bind::Bindable>> bindPtrs)
{
	if (!is_static_init())
	{
		ApplyStaticBind(make_unique<Bind::PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	for (unique_ptr<Bind::Bindable>& pBindable : bindPtrs)
	{
		if (Bind::IndexBuffer* pIndexBuffer = dynamic_cast< Bind::IndexBuffer*>(pBindable.get()))
		{
			ApplyIndexBuffer(unique_ptr<Bind::IndexBuffer>{ pIndexBuffer });
			pBindable.release();
		}
		else
		{
			ApplyBind(std::move(pBindable));
		}
	}

	ApplyBind(make_unique<Bind::TransformConstBuffer>(gfx, *this));
}

void Mesh::Render(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept_unless
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::Render(gfx);
}

DirectX::XMMATRIX Mesh::FetchTransformMat() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}

struct ModelWnd
{
private:
	struct
	{
		object_variables::orientation orientation;
		object_variables::position position;
	}object_var;
public:
	matrix FetchTransform() const noexcept
	{
		return
			DirectX::XMMatrixRotationRollPitchYaw
			(
				object_var.orientation.roll,
				object_var.orientation.pitch,
				object_var.orientation.yaw
			)
			*
			DirectX::XMMatrixTranslation(
				object_var.position.x,
				object_var.position.y,
				object_var.position.z
			);
	}
	void Present(const char* window, const Node& root) noexcept
	{
		if (!window)
			window = "Model";
		using namespace ImGui;
		if (Begin(window))
		{
			Columns(2, nullptr);
			root.RenderTree();


			NextColumn();
			Text("Orientation");
			SliderAngle("Roll", &object_var.orientation.roll, -180.0f, 180.0f);
			SliderAngle("Pitch", &object_var.orientation.pitch, -180.0f, 180.0f);
			SliderAngle("Yaw", &object_var.orientation.yaw, -180.0f, 180.0f);
			Text("Position");
			SliderFloat("X", &object_var.position.x, -20.0f, 20.0f);
			SliderFloat("Y", &object_var.position.y, -20.0f, 20.0f);
			SliderFloat("Z", &object_var.position.z, -20.0f, 20.0f);

			if (Button("Reset"))
			{
				object_var.position.x = 0;
				object_var.position.y = 0;
				object_var.position.z = 0;
				object_var.orientation.roll = 0;
				object_var.orientation.pitch = 0;
				object_var.orientation.yaw = 0;
			}
		}
		End();
	}
};

Model::Model(Graphics& gfx, const std::string fileName)
:
	pWnd (make_unique<ModelWnd>() )
{
	Assimp::Importer imp;
	const aiScene* const pScene =
		imp.ReadFile(
			fileName.c_str(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);

	for (size_t idx = 0; idx < pScene->mNumMeshes; idx++)
	{
		pMeshes.push_back(ParseMesh(gfx, *pScene->mMeshes[idx]));
	}

	pRoot = ParseNode(*pScene->mRootNode);
}
Model::~Model() noexcept
{}

void Model::Render(Graphics& gfx) const noexcept_unless
{
	pRoot->Render(gfx, pWnd->FetchTransform());
}

unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh)
{
	using DynamicVertex::VertexLayout;

	DynamicVertex::VertexBuffer VertexBuf(
		std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
		)
	);

	for (unsigned int idx = 0; idx < mesh.mNumVertices; idx++)
	{
		VertexBuf.Emplace_Back(
			*reinterpret_cast<float3*>(&mesh.mVertices[idx]),
			*reinterpret_cast<float3*>(&mesh.mNormals[idx])
		);
	}

	vector<unsigned short> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (unsigned int idx = 0; idx < mesh.mNumFaces; idx++)
	{
		const auto& face = mesh.mFaces[idx];
		assert(face.mNumIndices == 3);
		{
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

	}

	vector<unique_ptr<Bind::Bindable>> bindablePtrs;

	bindablePtrs.push_back(make_unique<Bind::VertexBuffer>(gfx, VertexBuf));

	bindablePtrs.push_back(make_unique<Bind::IndexBuffer>(gfx, indices));

	std::unique_ptr<Bind::VertexShader, std::default_delete<Bind::VertexShader>> pVertexShader = make_unique<Bind::VertexShader>(gfx, L"PhongShaderVS.cso");
	ID3DBlob* pvsbc = pVertexShader->FetchByteCode();
	bindablePtrs.push_back(std::move(pVertexShader));

	bindablePtrs.push_back(make_unique<Bind::PixelShader>(gfx, L"PhongShaderPS.cso"));

	bindablePtrs.push_back(make_unique<Bind::InputLayout>(gfx, VertexBuf.FetchLayout().FetchD3DLayout(), pvsbc));

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 colour = { 0.8f,0.2f,0.2f };
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} pixelMatConstant;
	bindablePtrs.push_back(make_unique<Bind::PixelConstantBuffer<PSMaterialConstant>>(gfx, pixelMatConstant, 1u));

	return make_unique<Mesh>(gfx, std::move(bindablePtrs));
}

unique_ptr<Node> Model::ParseNode(const aiNode& node) noexcept
{
	namespace DirectX = DirectX;
	const matrix transform = DirectX::XMMatrixTranspose(
		DirectX::XMLoadFloat4x4(
			reinterpret_cast<const float4x4*>(&node.mTransformation)
		)
	);

	vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t idx = 0; idx < node.mNumMeshes; idx++)
	{
		const uint32_t meshIdx = node.mMeshes[idx];
		curMeshPtrs.push_back(pMeshes.at(meshIdx).get());
	}

	std::unique_ptr<Node> pNode = make_unique<Node>(node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t idx = 0; idx < node.mNumChildren; idx++)
	{
		pNode->AddChild(ParseNode(*node.mChildren[idx]));
	}

	return pNode;
}

void Model::PresentWindow(const char* Window) noexcept
{
	pWnd->Present(Window, *pRoot);
}

void Node::RenderTree() const noexcept // recursively render child nodes
{
	using namespace ImGui;
	if (TreeNode(name.c_str()))
	{
		for (const std::unique_ptr<Node>& pChild : pChildren)
			pChild->RenderTree();

		TreePop();
	}
}