#include "Mesh.h"
#include <unordered_map>
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
	struct transform_params
	{
		object_variables::orientation orientation;
		object_variables::position position;
	};
	std::optional<int> idx_selected;
	Node* pNode_selected;
	std::unordered_map<int, transform_params> transform_map;
public:
	matrix FetchTransform() const noexcept
	{
		if (!pNode_selected)
		{
			const std::unordered_map<int, ModelWnd::transform_params>::mapped_type& transform
				= transform_map.at(* idx_selected);

				return
				DirectX::XMMatrixRotationRollPitchYaw
				(
					transform.orientation.roll,
					transform.orientation.pitch,
					transform.orientation.yaw
				)
				*
				DirectX::XMMatrixTranslation(
					transform.position.x,
					transform.position.y,
					transform.position.z
				);
		}
	}
	Node* FetchSelectedNode() const noexcept
	{
		return pNode_selected;
	}
	void Present(const char* window, const Node& root) noexcept
	{
		if (!window)
			window = "Model";
		using namespace ImGui;
		int nodeidx=0;
		if (Begin(window))
		{

			Columns(2, nullptr);
			root.RenderTree(nodeidx, idx_selected, pNode_selected);

			NextColumn();
			if (pNode_selected)
			{
				auto& transform
					= transform_map[*idx_selected];

				Text("Orientation");
				SliderAngle("Roll", &transform.orientation.roll, -180.0f, 180.0f);
				SliderAngle("Pitch", &transform.orientation.pitch, -180.0f, 180.0f);
				SliderAngle("Yaw", &transform.orientation.yaw, -180.0f, 180.0f);
				Text("Position");
				SliderFloat("X", &transform.position.x, -20.0f, 20.0f);
				SliderFloat("Y", &transform.position.y, -20.0f, 20.0f);
				SliderFloat("Z", &transform.position.z, -20.0f, 20.0f);

				if (Button("Reset"))
				{
					transform.position.x = 0;
					transform.position.y = 0;
					transform.position.z = 0;
					transform.orientation.roll = 0;
					transform.orientation.pitch = 0;
					transform.orientation.yaw = 0;
				}

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
	if (auto node = pWnd->FetchSelectedNode())
	{
		node->Transform_Apply(pWnd->FetchTransform());
	}
	pRoot->Render(gfx, DirectX::XMMatrixIdentity());
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

void Node::RenderTree(int& idx_node, std::optional<int>& idx_selected, Node*& pNode_selected) const noexcept // recursively render child nodes
{
	const int idx_node_current = idx_node; ++idx_node;
	
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((idx_node_current == idx_selected.value_or(-1)) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((pChildren.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);

	using namespace ImGui;
	if (TreeNodeEx((void*)(intptr_t)idx_node_current,node_flags,name.c_str()))
	{
		if (IsItemClicked())
		{
			idx_selected = idx_node_current;
			pNode_selected = const_cast<Node*>(this);
		}
		for (const std::unique_ptr<Node>& pChild : pChildren)
		{
			pChild->RenderTree(idx_node, idx_selected, pNode_selected);
		}
		TreePop();
	}
}

void Node::Transform_Apply(DirectX::FXMMATRIX transform) noexcept
{
	DirectX::XMStoreFloat4x4
	(
		&transform_applied,
		transform
	);
}
