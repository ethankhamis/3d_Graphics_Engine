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
	void Render(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept_unless;
	DirectX::XMMATRIX FetchTransformMat() const noexcept override;
private:
	mutable DirectX::XMFLOAT4X4 transform;
};

struct Node
{
	friend struct Model;

	Node(const std::string& name,vector<Mesh*> pMeshes, const DirectX::XMMATRIX& transform) noexcept_unless
		:
		pMeshes(std::move(pMeshes)),
			name(name)
	{
		DirectX::XMStoreFloat4x4(&this->transform, transform);
	}
		void Render(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept_unless
		{
			const matrix built = DirectX::XMLoadFloat4x4(&transform) * accumulatedTransform;
			for (Mesh* const pm : pMeshes)
			{
				pm->Render(gfx, built);
			}
			for (const unique_ptr<Node>& pChild : pChildren)
			{
				pChild->Render(gfx, built);
			}
		}
		void RenderTree() const noexcept;
private:
	void AddChild(unique_ptr<Node> pChild) noexcept_unless
	{
		assert(pChild);
		pChildren.push_back(std::move(pChild));
	}
private:
	vector<Mesh*> pMeshes;
	vector<unique_ptr<Node>> pChildren;
	DirectX::XMFLOAT4X4 transform;
	std::string name;
};

struct Model
{
	Model(Graphics& gfx, const std::string fileName);
	~Model() noexcept;
	void Render(Graphics& gfx) const noexcept_unless;
private:
	static unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh);
	unique_ptr<Node> ParseNode(const aiNode& node) noexcept;
public:
	void PresentWindow(const char* Window = nullptr) noexcept;
private:
	unique_ptr<Node> pRoot;
	vector<unique_ptr<Mesh>> pMeshes;
	unique_ptr<struct ModelWnd> pWnd;
};
