#pragma once
#include "randomTest.h"
#include "DefaultBindables.h"
#include "ThrowMacros.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "DynamicVertex.h"

using namespace Bind;

RandomTest::RandomTest(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, DirectX::XMFLOAT3 material, float scale)
	:
	TestObject(gfx,rng,adist,ddist,odist,rdist)
{

	if (!is_static_init())
	{
		using DynamicVertex::VertexLayout;
		DynamicVertex::VertexBuffer vertexBuffer(
			std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
		));
		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("models\\monkey.obj",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);
		const auto pMesh = pModel->mMeshes[0];

		
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertexBuffer.Emplace_Back(
				float3{ pMesh->mVertices[i].x * scale,pMesh->mVertices[i].y * scale,pMesh->mVertices[i].z * scale },
				*reinterpret_cast<float3*>(&pMesh->mNormals[i])
				);
		}

		std::vector<unsigned short> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		ApplyStaticBind(std::make_unique<VertexBuffer>(gfx, vertexBuffer));

		ApplyStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongShaderVS.cso");
		auto pvsbc = pvs->FetchByteCode();
		ApplyStaticBind(std::move(pvs));

		ApplyStaticBind(std::make_unique<PixelShader>(gfx, L"PhongShaderPS.cso"));

		
		ApplyStaticBind(std::make_unique<InputLayout>(gfx, vertexBuffer.FetchLayout().FetchD3DLayout(), pvsbc));

		ApplyStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3 color;
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} pmc;
		pmc.color = material;
		ApplyStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));
	}
	else
	{
		ApplyIndexFromStatic();
	}

	ApplyBind(std::make_unique<TransformConstBuffer>(gfx, *this));

}
