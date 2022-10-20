#pragma once
#include "MathematicalConstants.h"
#include "DynamicVertex.h"
#include "DefaultBindables.h"
#include "DrawableBase.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <optional>

using std::unique_ptr;
using std::make_unique;
using std::vector;

struct ModelException : public ExceptionHandler
{
	ModelException(int line, const wchar_t* filename, std::wstring note) noexcept;
	const wchar_t* whatw() const noexcept override;
	const wchar_t* FetchErrorType() const noexcept override;
	const std::wstring& FetchNote() const noexcept;
private:
	std::wstring note;
};

struct Mesh : public DrawableBase<Mesh>
{
	Mesh(Graphics& gfx, vector<unique_ptr<Bind::Bindable>> bindPtrs);
	void Render(Graphics& gfx, const matrix accumulatedTransform) const noexcept_unless;
	matrix FetchTransformMat() const noexcept override;
private:
	mutable float4x4 transform;
};

struct Node
{
private:
	friend struct Model;
	friend struct ModelWnd;
public:
	Node(const std::string& name, vector<Mesh*> pMeshes, const matrix& transform) noexcept_unless;
	void Render(Graphics& gfx, const matrix current_transform) const noexcept_unless;
	void Transform_Apply(const matrix transform) noexcept;
private:
	void RenderTree(int& idx_node /*id of node*/,
		std::optional<int>& idx_selected,
		Node*& pNode_selected) const noexcept;
	void AddChild(unique_ptr<Node> pChild) noexcept_unless;
private:
	vector<Mesh*> pMeshes;
	vector<unique_ptr<Node>> pChildren;
	float4x4 transform_initial;
	float4x4 transform_applied;
	std::string name;
};

struct Model
{
	Model(Graphics& gfx, const std::string fileName);
	void Render(Graphics& gfx) const noexcept_unless;
private:
	static unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh);
	unique_ptr<Node> ParseNode(const aiNode& node) noexcept;
public:
	void PresentWindow(const char* Window = nullptr) noexcept;
	~Model() noexcept;
private:
	unique_ptr<Node> pRoot;
	vector<unique_ptr<Mesh>> pMeshes;
	unique_ptr<struct ModelWnd> pWnd;
};
