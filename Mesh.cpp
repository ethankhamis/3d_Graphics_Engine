#include "Mesh.h"

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

void Mesh::Render(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::Render(gfx);
}

DirectX::XMMATRIX Mesh::FetchTransformMat() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}

Model::Model(Graphics& gfx, const std::string fileName)
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

unique_ptr<Node> Model::ParseNode(const aiNode& node)
{
	namespace DirectX = DirectX;
	const matrix transform = DirectX::XMMatrixTranspose(
		DirectX::XMLoadFloat4x4(
			reinterpret_cast<const DirectX::XMFLOAT4X4*>(&node.mTransformation)
		)
	);

	vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t idx = 0; idx < node.mNumMeshes; idx++)
	{
		const uint32_t meshIdx = node.mMeshes[idx];
		curMeshPtrs.push_back(pMeshes.at(meshIdx).get());
	}

	std::unique_ptr<Node> pNode = make_unique<Node>(std::move(curMeshPtrs), transform);
	for (size_t idx = 0; idx < node.mNumChildren; idx++)
	{
		pNode->AddChild(ParseNode(*node.mChildren[idx]));
	}

	return pNode;
}
