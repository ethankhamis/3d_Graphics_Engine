#include "Mesh.h"
#include <unordered_map>
#include "imgui/imgui.h"
#include <sstream>
#include "Surface.h"
#include <filesystem>
#include "Converter.h"
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

void Node::AppendChildNode(unique_ptr<Node> pChild) noexcept_unless
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
	int idx_selected = 0;
	if (pNode_selected == nullptr)
	{
		  idx_selected = -1;
	}
	else { idx_selected = pNode_selected->Fetch_id(); }

	int isselectedNode = NULL;
	int isleafNode = NULL;

	if (Fetch_id() == idx_selected) { isselectedNode = ImGuiTreeNodeFlags_Selected; }
	if (pChildren.size() == NULL) { isleafNode = ImGuiTreeNodeFlags_Leaf; }


	const int node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| isselectedNode | isleafNode;

	const auto expanded =
		ImGui::TreeNodeEx(
			(void*)(int)Fetch_id(), node_flags, name.c_str()
		);

	if (ImGui::IsItemClicked())
	{
		pNode_selected = const_cast<Node*>(this);
	}
	if (expanded)
	{
		for (const std::unique_ptr<Node>& pChild : pChildren)
		{
			pChild->RenderTree( pNode_selected);
		}
		ImGui::TreePop(); // exit tree 
		// TreePop is called once the tree is opened
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
			//state three seperate columns for window
			Columns(3, nullptr);
			//render node of windows
			root.RenderTree( pNode_selected);

				//if current node is selected
				if (pNode_selected)
				{
					//access reference to the transformation data stored within hash map
					auto& transform =
						transform_map[pNode_selected->Fetch_id()];
					//provide next column
					NextColumn();
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
					//provide next column
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
		//close window
		End();
	}
};

Model::Model(Graphics& gfx, const std::wstring& fileName)
:
	pWnd (make_unique<ModelWnd>() )
{
	Assimp::Importer imp;
	const aiScene* const pScene =
		imp.ReadFile(
			convert::make_string(fileName.c_str()),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace
		);
	if (pScene == nullptr)
	{
		std::wstring ws = convert::make_wstring(imp.GetErrorString());
		throw ModelException(__LINE__, WFILE, ws.c_str());
	}

		for (size_t idx = 0; idx < pScene->mNumMeshes; ++idx)
		{
			pMeshes.push_back(AppendMesh(gfx, *pScene->mMeshes[idx],pScene->mMaterials, fileName));
		}
	unsigned int next_id = 0;
	pRoot = AppendNode(next_id,*pScene->mRootNode);
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

unique_ptr<Mesh> Model::AppendMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials,const std::filesystem::path& fpath)
{
	using DynamicVertex::VertexLayout;

	using namespace std::string_literals;
	std::vector<std::shared_ptr<Bind::isBinded>> bindablePtrs;
	const std::wstring root = fpath.parent_path().wstring() + L"\\";


	bool contains_specular = false;
	bool contains_normal = false;
	bool contains_diffuse = false;
	float shininess = 35.f;

	float4 diffuse_colour = { .5, .5,.85, 1.f };
	float4 specular_colour = { .2,.2,.2,1.f };

	if (mesh.mMaterialIndex >= NULL) // if  materials exist in the mesh
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];
		aiString texture_filename;
		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texture_filename) == aiReturn_SUCCESS)
		{
			//emplace texture bind as a shared_ptr
			bindablePtrs.push_back(Bind::Texture::Store(gfx, root + convert::make_wstring(texture_filename.C_Str())));
			contains_diffuse = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuse_colour));
		}

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texture_filename) == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Bind::Texture::Store(gfx, root + convert::make_wstring(texture_filename.C_Str()), 1));
			contains_specular = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specular_colour));
		}

		if (material.GetTexture(aiTextureType_NORMALS, 0, &texture_filename) == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Bind::Texture::Store(gfx, root + convert::make_wstring((texture_filename.C_Str())), 2));
			contains_normal = true;
		}

		if (contains_diffuse || contains_specular || contains_normal)
		{
			bindablePtrs.push_back(Bind::Sampler::Store(gfx));
		}
	}
	std::wstring mesh_name = convert::make_wstring(fpath.string().c_str()) + L"%" + convert::make_wstring(mesh.mName.C_Str());
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
			float specular_intensity;
			float specular_power;
			int normal_map_state = 1;
			float padding[1];
		} pixelMatConstant;
		// MUST BE ADDRESSED
		//  + all meshes share the same material constant despite potentially having different specular power (based on material) in their material properties
		pixelMatConstant.specular_power = shininess;
		pixelMatConstant.specular_intensity = (specular_colour.x + specular_colour.y + specular_colour.z) / 3.f;
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
			float specular_intensity = 0.18f;
			float specularPower;
			float padding[2];
		} pixelMatConstant;
		// MUST BE ADDRESSED
		//  + all meshes share the same material constant despite potentially having different specular power (based on material) in their material properties
		pixelMatConstant.specularPower = shininess;
		pixelMatConstant.specular_intensity = (specular_colour.x  + specular_colour.y + specular_colour.z) / 3.f;
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
			float4 colour;
			float specularIntensity;
			float specularPower;
			float padding[2];
		} pixelMatConstant;
		// MUST BE ADDRESSED
		//  + all meshes share the same material constant despite potentially having different specular power (based on material) in their material properties
		pixelMatConstant.specularPower = shininess;
		pixelMatConstant.colour = diffuse_colour;
		pixelMatConstant.specularIntensity = (specular_colour.x + specular_colour.y + specular_colour.z) / 3.f;
		bindablePtrs.push_back(Bind::PixelConstantBuffer<PSMaterialConstant_untextured>::Store(gfx, pixelMatConstant, 1u));
	}
	else
{
	throw std::runtime_error("unusable textures");
}
	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}

unique_ptr<Node> Model::AppendNode(unsigned int& next_id,const aiNode& node) noexcept
{
	//create transform converting assimp transform to row-major matrix
	const matrix transform = DirectX::XMMatrixTranspose(
		DirectX::XMLoadFloat4x4(
			reinterpret_cast<const float4x4*>(&node.mTransformation)
		)
	);
	// create a vector of mesh pointers for the current node to use
	vector<Mesh*> curMeshPtrs;
	//allocate stack memory for the vector
	curMeshPtrs.reserve(node.mNumMeshes);
	//loop through each mesh within the node
	for (size_t idx = 0; idx < node.mNumMeshes; idx++)
	{
		//find the index to the mesh
		const uint32_t meshIdx = node.mMeshes[idx];
		//return reference to mesh at position meshIdx and emplace the mesh pointer to the vector of meshes for the node
		curMeshPtrs.push_back(pMeshes.at(meshIdx).get());
	}
	// construct node using gathered data
	std::unique_ptr<Node> pNode = make_unique<Node>(next_id++,node.mName.C_Str(), std::move(curMeshPtrs), transform);

	//loop through nodes children
	for (size_t idx = 0; idx < node.mNumChildren; idx++)
	{
		//recursively add child node
		pNode->AppendChildNode(AppendNode(next_id,*node.mChildren[idx]));
	}

	return pNode;
}

void Model::PresentWindow(const char* Window) noexcept
{
	pWnd->Present(Window, *pRoot);
}

ModelException::ModelException(int line, const std::wstring filename, std::wstring note) noexcept
:
ExceptionHandler(line,filename.c_str()), note(std::move(note))
{}

const wchar_t* ModelException::whatw() const noexcept
{
	std::wstringstream wss;
	wss << ExceptionHandler::whatw() << std::endl
		<< L"\n[Note]\n" << FetchNote().c_str();
	buffer_w = wss.str().c_str();
	return buffer_w.c_str();
}

const wchar_t* ModelException::FetchErrorType() const noexcept
{
	return L"Model Exception";
}

const std::wstring& ModelException::FetchNote() const noexcept
{
	return note;
}
