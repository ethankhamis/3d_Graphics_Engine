#pragma once
#include "Math.cpp"
#include "DynamicVertex.h"
#include "DefaultBinds.h"
#include "isRendered.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <optional>
#include <filesystem>

using std::unique_ptr;
using std::make_unique;
using std::vector;

struct ModelException : public ExceptionHandler
{
	ModelException(int line, const std::wstring filename, std::wstring note) noexcept;
	const wchar_t* whatw() const noexcept override;
	const wchar_t* FetchErrorType() const noexcept override;
	const std::wstring& FetchNote() const noexcept;
private:
	std::wstring note;
};

struct Mesh : public isRendered
{
	Mesh(Graphics& gfx, vector<std::shared_ptr<Bind::isBinded>> bindPtrs);
	void Render(Graphics& gfx, const matrix accumulatedTransform) const noexcept_unless;
	matrix FetchTransformMat() const noexcept override;
private:
	mutable float4x4 transform;
};

struct Node
{
private:
	friend struct Model;
public:
	Node(unsigned int id ,const std::string& name, vector<Mesh*> pMeshes, const matrix& transform) noexcept_unless;
	void Render(Graphics& gfx, const matrix current_transform) const noexcept_unless;
	void Transform_Apply(const matrix transform) noexcept;
	int Fetch_id() const noexcept;
	void RenderTree(
		Node*& pNode_selected) const noexcept;
private:
	void AppendChildNode(unique_ptr<Node> pChild) noexcept_unless;
private:
	unsigned int id;
	vector<Mesh*> pMeshes;
	vector<unique_ptr<Node>> pChildren;
	float4x4 transform_initial;
	float4x4 transform_applied;
	std::string name;
};

struct Model
{
	Model(Graphics& gfx, const std::wstring& fileName);
	void Render(Graphics& gfx) const noexcept_unless;
	void SetTransformation(const matrix transform);
private:
	static unique_ptr<Mesh> AppendMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials,const std::filesystem::path& path);
	unique_ptr<Node> AppendNode(unsigned int& next_id,const aiNode& node) noexcept;
public:
	void PresentWindow(const char* Window = nullptr) noexcept;
	~Model() noexcept;
private:
	unique_ptr<Node> pRoot;
	vector<unique_ptr<Mesh>> pMeshes;
	unique_ptr<struct ModelWnd> pWnd;
};
