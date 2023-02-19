#include "Mesh.h"
#include <unordered_map>
#include "imgui/imgui.h"
#include <sstream>
#include "Surface.h"
Mesh::Mesh(Graphics& gfx, vector<std::shared_ptr<Bind::isBinded>> pBinds)
{
	
	ApplyBind(Bind::PrimTopology::Store(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


	for (std::shared_ptr<Bind::isBinded>& pBindable : pBinds)
	{
		
		ApplyBind(std::move(pBindable));

	}

	ApplyBind(std::make_shared<Bind::TransformConstBuffer>(gfx, *this));
}

void Mesh::Render(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept_unless
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	isRendered::Render(gfx);
}

DirectX::XMMATRIX Mesh::FetchTransformMat() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}


Node::Node(unsigned int id,const std::string& name, std::vector<Mesh*> pMeshes, const matrix& transform) noexcept_unless
	:
	id(id),
	pMeshes(std::move(pMeshes)),
	name(name)
{
	DirectX::XMStoreFloat4x4(&transform_initial, transform); // store initial transform
	DirectX::XMStoreFloat4x4(&transform_applied, DirectX::XMMatrixIdentity()); // store final transform *applied*
}

void Node::AddChild(unique_ptr<Node> pChild) noexcept_unless
{
	assert(pChild);
	pChildren.push_back(std::move(pChild));
}
void Node::Render(Graphics& gfx, DirectX::FXMMATRIX current_transform) const noexcept_unless
{
	const matrix transform_built =
		DirectX::XMLoadFloat4x4(&transform_applied) *
		DirectX::XMLoadFloat4x4(&transform_initial) *
		current_transform;

	for (const Mesh* pm : pMeshes)
	{
		pm->Render(gfx, transform_built);
	}
	for (const unique_ptr<Node>& pChild : pChildren)
	{
		pChild->Render(gfx, transform_built);
	}
}

void Node::RenderTree(Node*& pNode_selected) const noexcept // recursively render child nodes
{
	const int idx_selected = (pNode_selected == nullptr) ? -1 : pNode_selected->Fetch_id();

	const int node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((Fetch_id() == idx_selected) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((pChildren.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);

	using namespace ImGui;
	const auto expanded =
		TreeNodeEx(
			(void*)(int)Fetch_id(), node_flags, name.c_str()
		);

	if (IsItemClicked())
	{
		pNode_selected = const_cast<Node*>(this);
	}
	if (expanded)
	{
		for (const std::unique_ptr<Node>& pChild : pChildren)
		{
			pChild->RenderTree( pNode_selected);
		}
		TreePop();
	}
}

int Node::Fetch_id() const noexcept
{
	return id;
}

void Node::Transform_Apply(DirectX::FXMMATRIX transform) noexcept
{
	DirectX::XMStoreFloat4x4
	(
		&transform_applied,
		transform
	);
}

struct ModelWnd
{
private:
	std::optional<int> idx_selected;
	Node* pNode_selected;
	struct transform_params
	{
		object_variables::orientation orientation;
		object_variables::position position;
	};
	std::unordered_map<int, transform_params> transform_map;
public:
	matrix FetchTransform() const noexcept
	{
			assert(pNode_selected != nullptr);
			const std::unordered_map<int, ModelWnd::transform_params>::mapped_type& transform
				= transform_map.at(pNode_selected->Fetch_id());

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
	Node* FetchSelectedNode() const noexcept
	{
		return pNode_selected;
	}
	void Present(const char* window, const Node& root) noexcept
	{
		if (!window)
			window = "Model";
		using namespace ImGui;
		int nodeidx = 0;
		if (Begin(window))
		{

			Columns(3, nullptr);
			root.RenderTree( pNode_selected);

				NextColumn();
				if (pNode_selected)
				{
					auto& transform =
						transform_map[pNode_selected->Fetch_id()];

					Text("Position");
					SliderFloat("X", &transform.position.x, -20.0f, 20.0f);
					SliderFloat("Y", &transform.position.y, -20.0f, 20.0f);
					SliderFloat("Z", &transform.position.z, -20.0f, 20.0f);

					if (Button("Reset Position"))
					{
						transform.position.x = 0;
						transform.position.y = 0;
						transform.position.z = 0;
					}

					NextColumn();

					Text("Orientation");
					SliderAngle("Roll", &transform.orientation.roll, -180.0f, 180.0f);
					SliderAngle("Pitch", &transform.orientation.pitch, -180.0f, 180.0f);
					SliderAngle("Yaw", &transform.orientation.yaw, -180.0f, 180.0f);

					if (Button("Reset Orientation"))
					{
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
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace
		);
	if (pScene == nullptr)
	{
		// Convert error string to wide
		std::string  cs(imp.GetErrorString());
		std::wstring ws;
		copy(cs.begin(), cs.end(), back_inserter(ws));
		throw ModelException(__LINE__, WFILE, ws);
	}
		for (size_t idx = 0; idx < pScene->mNumMeshes; ++idx)
		{
			pMeshes.push_back(ParseMesh(gfx, *pScene->mMeshes[idx],pScene->mMaterials));
		}
	unsigned int next_id = 0;
	pRoot = ParseNode(next_id,*pScene->mRootNode);
}
void Model::SetTransformation(matrix transform)
{
	pRoot->Transform_Apply(transform);
}
Model::~Model() noexcept
{}

void Model::Render(Graphics& gfx) const noexcept_unless
{
	if (Node* node = pWnd->FetchSelectedNode())
	{
		node->Transform_Apply(pWnd->FetchTransform());
	}
	pRoot->Render(gfx, DirectX::XMMatrixIdentity());
}

unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials)
{
	using DynamicVertex::VertexLayout;

	using namespace std::string_literals;
	std::vector<std::shared_ptr<Bind::isBinded>> bindablePtrs;
	const std::wstring base = L"Models\\miles\\"s;


	bool contains_specular = false;
	bool contains_normal = false;
	bool contains_diffuse = false;
	float shininess = 35.f;
	if (mesh.mMaterialIndex >= NULL)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];
		aiString texture_filename;
		std::wstring texture_filename_w;
		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texture_filename) == aiReturn_SUCCESS)
		{
			std::string temp = texture_filename.C_Str();
			std::copy(temp.begin(), temp.end(), back_inserter(texture_filename_w));
			bindablePtrs.push_back(Bind::Texture::Store(gfx, base + texture_filename_w));
			texture_filename_w.clear();
			contains_diffuse = true;
		}

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texture_filename) == aiReturn_SUCCESS)
		{
			std::string temp = texture_filename.C_Str();
			std::copy(temp.begin(), temp.end(), back_inserter(texture_filename_w));
			bindablePtrs.push_back(Bind::Texture::Store(gfx, base + texture_filename_w, 1));
			texture_filename_w.clear();
			contains_specular = true;
		}
		else
		{
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		if (material.GetTexture(aiTextureType_NORMALS, 0, &texture_filename) == aiReturn_SUCCESS)
		{
			std::string temp = texture_filename.C_Str();
			std::copy(temp.begin(), temp.end(), back_inserter(texture_filename_w));
			bindablePtrs.push_back(Bind::Texture::Store(gfx, base + texture_filename_w, 2));
			texture_filename_w.clear();
			contains_normal = true;
		}

		if (contains_diffuse || contains_specular || contains_normal)
		{
			bindablePtrs.push_back(Bind::Sampler::Store(gfx));
		}
	}
	std::wstring mesh_name = base + L"%" + convert::make_wstring(mesh.mName.C_Str());
	const float scale = 6.f;

	if (contains_diffuse && contains_normal && contains_specular)
	{
		DynamicVertex::VertexBuffer VertexBuf(
			std::move(
				VertexLayout{}
				.Emplace_Back(VertexLayout::Position3D)
				.Emplace_Back(VertexLayout::Normal)
				.Emplace_Back(VertexLayout::Tangent)
				.Emplace_Back(VertexLayout::Bitangent)
				.Emplace_Back(VertexLayout::Texture2D)
			)
		);

		for (unsigned int idx = 0; idx < mesh.mNumVertices; idx++)
		{
			VertexBuf.Emplace_Back(
				float3(mesh.mVertices[idx].x * scale, mesh.mVertices[idx].y * scale, mesh.mVertices[idx].z * scale),
				*reinterpret_cast<float3*>(&mesh.mNormals[idx]),
				*reinterpret_cast<float3*>(&mesh.mTangents[idx]),
				*reinterpret_cast<float3*>(&mesh.mBitangents[idx]),
				*reinterpret_cast<float2*>(&mesh.mTextureCoords[NULL][idx])
			);
		}

		vector<UINT16> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int idx = 0; idx < mesh.mNumFaces; idx++)
		{
			const auto& face = mesh.mFaces[idx];
			assert(face.mNumIndices == 3);

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);

		}
		bindablePtrs.push_back(Bind::VertexBuffer::Store(gfx, mesh_name, VertexBuf));

		bindablePtrs.push_back(Bind::IndexBuffer::Store(gfx, mesh_name, indices));

		/*std::unique_ptr<Bind::VertexShader, std::default_delete<Bind::VertexShader>>*/
		auto pVertexShader = Bind::VertexShader::Store(gfx, L"PhongShaderNormalVS.cso");
		ID3DBlob* pVertexShaderByteCode = pVertexShader->FetchByteCode();
		bindablePtrs.push_back(std::move(pVertexShader));

		bindablePtrs.push_back(Bind::PixelShader::Store(gfx, L"PhongShaderPSSpecularNormalMap.cso"));
		bindablePtrs.push_back(Bind::InputLayout::Store(gfx, VertexBuf.FetchLayout(), pVertexShaderByteCode));

		struct PixelShaderMaterialConstant_dns
		{
			int normal_map_state = 1;
			float padding[3];
		}pixelmatconst;
		//extremely bad as all meshes will share the same material const
		bindablePtrs.push_back(Bind::PixelConstantBuffer<PixelShaderMaterialConstant_dns>::Store(gfx, pixelmatconst, 1u));
	}

	else if (contains_diffuse && contains_normal)
	{
		DynamicVertex::VertexBuffer VertexBuf(
			std::move(
				VertexLayout{}
				.Emplace_Back(VertexLayout::Position3D)
				.Emplace_Back(VertexLayout::Normal)
				.Emplace_Back(VertexLayout::Tangent)
				.Emplace_Back(VertexLayout::Bitangent)
				.Emplace_Back(VertexLayout::Texture2D)
			)
		);

		for (unsigned int idx = 0; idx < mesh.mNumVertices; idx++)
		{
			VertexBuf.Emplace_Back(
				float3(mesh.mVertices[idx].x * scale, mesh.mVertices[idx].y * scale, mesh.mVertices[idx].z * scale),
				*reinterpret_cast<float3*>(&mesh.mNormals[idx]),
				*reinterpret_cast<float3*>(&mesh.mTangents[idx]),
				*reinterpret_cast<float3*>(&mesh.mBitangents[idx]),
				*reinterpret_cast<float2*>(&mesh.mTextureCoords[NULL][idx])
			);
		}

		vector<UINT16> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int idx = 0; idx < mesh.mNumFaces; idx++)
		{
			const auto& face = mesh.mFaces[idx];
			assert(face.mNumIndices == 3);

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);

		}
		bindablePtrs.push_back(Bind::VertexBuffer::Store(gfx, mesh_name, VertexBuf));

		bindablePtrs.push_back(Bind::IndexBuffer::Store(gfx, mesh_name, indices));

		/*std::unique_ptr<Bind::VertexShader, std::default_delete<Bind::VertexShader>>*/
		auto pVertexShader = Bind::VertexShader::Store(gfx, L"PhongShaderNormalVS.cso");
		ID3DBlob* pVertexShaderByteCode = pVertexShader->FetchByteCode();
		bindablePtrs.push_back(std::move(pVertexShader));
		bindablePtrs.push_back(Bind::PixelShader::Store(gfx, L"PhongShaderNormalPS.cso"));
		bindablePtrs.push_back(Bind::InputLayout::Store(gfx, VertexBuf.FetchLayout(), pVertexShaderByteCode));

		struct PSMaterialConstant_Dn
		{
			float specularIntensity = 0.18f;
			float specularPower;
			int normal_map_state = 1;
			float padding[1];
		} pixelMatConstant;
		// MUST BE ADDRESSED
		//  + all meshes share the same material constant despite potentially having different specular power (based on material) in their material properties
		pixelMatConstant.specularPower = shininess;
		bindablePtrs.push_back(Bind::PixelConstantBuffer<PSMaterialConstant_Dn>::Store(gfx, pixelMatConstant, 1u));
	}
	else if (contains_diffuse)
	{
		DynamicVertex::VertexBuffer VertexBuf(
			std::move(
				VertexLayout{}
				.Emplace_Back(VertexLayout::Position3D)
				.Emplace_Back(VertexLayout::Normal)
				.Emplace_Back(VertexLayout::Texture2D)
			)
		);

		for (unsigned int idx = 0; idx < mesh.mNumVertices; idx++)
		{
			VertexBuf.Emplace_Back(
				float3(mesh.mVertices[idx].x * scale, mesh.mVertices[idx].y * scale, mesh.mVertices[idx].z * scale),
				*reinterpret_cast<float3*>(&mesh.mNormals[idx]),
				*reinterpret_cast<float2*>(&mesh.mTextureCoords[NULL][idx])
			);
		}

		vector<UINT16> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int idx = 0; idx < mesh.mNumFaces; idx++)
		{
			const auto& face = mesh.mFaces[idx];
			assert(face.mNumIndices == 3);

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);

		}

		bindablePtrs.push_back(Bind::VertexBuffer::Store(gfx, mesh_name, VertexBuf));

		bindablePtrs.push_back(Bind::IndexBuffer::Store(gfx, mesh_name, indices));

		/*std::unique_ptr<Bind::VertexShader, std::default_delete<Bind::VertexShader>>*/
		auto pVertexShader = Bind::VertexShader::Store(gfx, L"PhongShaderVS.cso");
		ID3DBlob* pVertexShaderByteCode = pVertexShader->FetchByteCode();
		bindablePtrs.push_back(std::move(pVertexShader));
		bindablePtrs.push_back(Bind::PixelShader::Store(gfx, L"PhongShaderPS.cso"));
		bindablePtrs.push_back(Bind::InputLayout::Store(gfx, VertexBuf.FetchLayout(), pVertexShaderByteCode));

		struct PSMaterialConstant_D
		{
			float specularIntensity = 0.18f;
			float specularPower;
			float padding[2];
		} pixelMatConstant;
		// MUST BE ADDRESSED
		//  + all meshes share the same material constant despite potentially having different specular power (based on material) in their material properties
		pixelMatConstant.specularPower = shininess;
		bindablePtrs.push_back(Bind::PixelConstantBuffer<PSMaterialConstant_D>::Store(gfx, pixelMatConstant, 1u));
	}

	else if (!contains_diffuse && !contains_normal && !contains_specular)
	{


		DynamicVertex::VertexBuffer VertexBuf(
			std::move(
				VertexLayout{}
				.Emplace_Back(VertexLayout::Position3D)
				.Emplace_Back(VertexLayout::Normal)
			)
		);

		for (unsigned int idx = 0; idx < mesh.mNumVertices; idx++)
		{
			VertexBuf.Emplace_Back(
				float3(mesh.mVertices[idx].x * scale, mesh.mVertices[idx].y * scale, mesh.mVertices[idx].z * scale),
				*reinterpret_cast<float3*>(&mesh.mNormals[idx])
			);
		}

		vector<UINT16> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int idx = 0; idx < mesh.mNumFaces; idx++)
		{
			const auto& face = mesh.mFaces[idx];
			assert(face.mNumIndices == 3);

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);

		}

		bindablePtrs.push_back(Bind::VertexBuffer::Store(gfx, mesh_name, VertexBuf));

		bindablePtrs.push_back(Bind::IndexBuffer::Store(gfx, mesh_name, indices));

		/*std::unique_ptr<Bind::VertexShader, std::default_delete<Bind::VertexShader>>*/
		auto pVertexShader = Bind::VertexShader::Store(gfx, L"PhongShaderVSUntextured.cso");
		ID3DBlob* pVertexShaderByteCode = pVertexShader->FetchByteCode();
		bindablePtrs.push_back(std::move(pVertexShader));
		bindablePtrs.push_back(Bind::PixelShader::Store(gfx, L"PhongShaderPSUntextured.cso"));
		bindablePtrs.push_back(Bind::InputLayout::Store(gfx, VertexBuf.FetchLayout(), pVertexShaderByteCode));

		struct PSMaterialConstant_untextured
		{
			float4 colour = { 0.5,0.5,0.85,1.f };
			float specularIntensity = 0.18f;
			float specularPower;
			float padding[2];
		} pixelMatConstant;
		// MUST BE ADDRESSED
		//  + all meshes share the same material constant despite potentially having different specular power (based on material) in their material properties
		pixelMatConstant.specularPower = shininess;
		bindablePtrs.push_back(Bind::PixelConstantBuffer<PSMaterialConstant_untextured>::Store(gfx, pixelMatConstant, 1u));
	}

	else
{
	throw std::runtime_error("unusable textures");
}

	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}

unique_ptr<Node> Model::ParseNode(unsigned int& next_id,const aiNode& node) noexcept
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

	std::unique_ptr<Node> pNode = make_unique<Node>(next_id++,node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t idx = 0; idx < node.mNumChildren; idx++)
	{
		pNode->AddChild(ParseNode(next_id,*node.mChildren[idx]));
	}

	return pNode;
}

void Model::PresentWindow(const char* Window) noexcept
{
	pWnd->Present(Window, *pRoot);
}

ModelException::ModelException(int line, const wchar_t* filename, std::wstring note) noexcept
:
ExceptionHandler(line,filename), note(std::move(note))
{}

const wchar_t* ModelException::whatw() const noexcept
{
	std::wstringstream wss;
	wss << ExceptionHandler::whatw() << std::endl
		<< L"\n[Note]\n" << FetchNote();
	std::wstring ws = wss.str();
	return ws.c_str();
}

const wchar_t* ModelException::FetchErrorType() const noexcept
{
	return L"Model Exception";
}

const std::wstring& ModelException::FetchNote() const noexcept
{
	return note;
}
