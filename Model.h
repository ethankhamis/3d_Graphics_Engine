#pragma once
#include "MathematicalConstants.h"
#include "DynamicVertex.h"
#include "DefaultBindables.h"
#include "DrawableBase.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using std::unique_ptr;
using std::make_unique;
using std::vector;

struct Mesh : public DrawableBase<Mesh>
{
	Mesh(Graphics& gfx, vector<unique_ptr<Bindable>> bindPtrs)
	{
		if (!is_static_init())
		{
			ApplyStaticBind(make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		for (unique_ptr<Bindable>& pBindable : bindPtrs)
		{
			if (IndexBuffer* pIndexBuffer = dynamic_cast<IndexBuffer*>(pBindable.get()))
			{
				ApplyIndexBuffer(unique_ptr<IndexBuffer>{ pIndexBuffer });
				pBindable.release();
			}
			else
			{
				ApplyBind(std::move(pBindable));
			}
		}

		ApplyBind(make_unique<TransformConstBuffer>(gfx, *this));
	}
	void Render(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept(!Debug)
	{
		DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
		Drawable::Render(gfx);
	}
	DirectX::XMMATRIX FetchTransformMat() const noexcept override
	{
		return DirectX::XMLoadFloat4x4(&transform);
	}
private:
	mutable DirectX::XMFLOAT4X4 transform;
};

struct Node
{
	friend struct Model;

	Node(vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform) noexcept(!Debug)
		:
		pMeshes(std::move(pMeshes))
	{
		DirectX::XMStoreFloat4x4(&this->transform, transform);
	}
	void Render(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept(!Debug)
	{
		const matrix built = DirectX::XMLoadFloat4x4(&transform) * accumulatedTransform;
		for (const auto pm : pMeshes)
		{
			pm->Render(gfx, built);
		}
		for (const unique_ptr<Node>& pChild : pChildren)
		{
			pChild->Render(gfx, built);
		}
	}
private:
	void AddChild(unique_ptr<Node> pChild) noexcept(!Debug)
	{
		assert(pChild);
		pChildren.push_back(std::move(pChild));
	}

	vector<Mesh*> pMeshes;
	vector<unique_ptr<Node>> pChildren;
	DirectX::XMFLOAT4X4 transform;
};

struct Model
{
	Model(Graphics& gfx, const std::string fileName)
	{
		Assimp::Importer imp;
		const aiScene *const pScene =
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
	static unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh)
	{
		namespace dx = DirectX;
		using experimental::VertexLayout;

		experimental::VertexBuffer VertexBuf(
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

		vector<unique_ptr<Bindable>> bindablePtrs;

		bindablePtrs.push_back(make_unique<VertexBuffer>(gfx, VertexBuf));

		bindablePtrs.push_back(make_unique<IndexBuffer>(gfx, indices));

		auto pvs = make_unique<VertexShader>(gfx, L"PhongShaderVS.cso");
		auto pvsbc = pvs->FetchByteCode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(make_unique<PixelShader>(gfx, L"PhongShaderPS.cso"));

		bindablePtrs.push_back(make_unique<InputLayout>(gfx, VertexBuf.FetchLayout().FetchD3DLayout(), pvsbc));

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3 colour = { 0.6f,0.6f,0.8f };
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} pmc;
		bindablePtrs.push_back(make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));

		return make_unique<Mesh>(gfx, std::move(bindablePtrs));
	}
	unique_ptr<Node> ParseNode(const aiNode& node)
	{
		namespace dx = DirectX;
		const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
			reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
		));

		vector<Mesh*> curMeshPtrs;
		curMeshPtrs.reserve(node.mNumMeshes);
		for (size_t idx = 0; idx < node.mNumMeshes; idx++)
		{
			const auto meshIdx = node.mMeshes[idx];
			curMeshPtrs.push_back(pMeshes.at(meshIdx).get());
		}

		auto pNode = make_unique<Node>(std::move(curMeshPtrs), transform);
		for (size_t idx = 0; idx < node.mNumChildren; idx++)
		{
			pNode->AddChild(ParseNode(*node.mChildren[idx]));
		}

		return pNode;
	}
	void Render(Graphics& gfx) const
	{
		pRoot->Render(gfx, DirectX::XMMatrixIdentity());
	}
private:
	unique_ptr<Node> pRoot;
	vector<unique_ptr<Mesh>> pMeshes;
};
