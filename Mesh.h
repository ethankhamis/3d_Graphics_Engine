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
	Mesh(Graphics& gfx, vector<unique_ptr<Bind::Bindable>> bindPtrs);
	void Render(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;
	DirectX::XMMATRIX FetchTransformMat() const noexcept override;
private:
	mutable DirectX::XMFLOAT4X4 transform;
};

struct Node
{
	friend struct Model;

	Node(vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform) noexcept_unless
		:
		pMeshes(std::move(pMeshes))
	{
		DirectX::XMStoreFloat4x4(&this->transform, transform);
	}
		void Render(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept_unless
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
	void AddChild(unique_ptr<Node> pChild) noexcept_unless
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
	Model(Graphics& gfx, const std::string fileName);
	void Render(Graphics& gfx, DirectX::FXMMATRIX transform) const
	{
		pRoot->Render(gfx, transform);
	}
	static unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh);
	unique_ptr<Node> ParseNode(const aiNode& node);

private:
	unique_ptr<Node> pRoot;
	vector<unique_ptr<Mesh>> pMeshes;
};
