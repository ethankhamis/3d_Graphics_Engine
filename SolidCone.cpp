#pragma once
#include "SolidCone.h"
#include "DefaultBindables.h"
#include "ThrowMacros.h"
#include "Cone.h"
#include <array>

SolidCone::SolidCone(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_int_distribution<int>& tdist)
	:
	TestObject(gfx,rng,adist,ddist,odist,rdist)
{
	if (!is_static_init())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			std::array<char, 4> colour;
			char padding;
		};

		const int ntessalated = tdist(rng);

		IndexedTriangleList<Vertex> model = Cone::Create_Advanced_IndependentFaces<Vertex>(ntessalated);

		//set mesh colours

		for (auto& vertex : model.vertices)
		{
			vertex.colour =
			{
				static_cast<char>(255),
				static_cast<char>(255),
				static_cast<char>(255)
			};
		}
		for (UINT idx = 0; idx < ntessalated; idx++)
		{
			model.vertices[idx*3].colour = //tip
			{
				static_cast<char>(255),
				static_cast<char>(0),
				static_cast<char>(0)
			};
		}

		model.Transform(DirectX::XMMatrixScaling(1.f, 1.f, .8f));

		model.ApplyNormalsIndependent();

		ApplyStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		std::unique_ptr<VertexShader> pvs = std::make_unique<VertexShader>(gfx, L"BlendedPhongShaderVS.cso");
		ID3DBlob* pvsbc = pvs->FetchByteCode();
		ApplyStaticBind(std::move(pvs));
		ApplyStaticBind(std::make_unique<PixelShader>(gfx, L"BlendedPhongShaderPS.cso"));

		ApplyStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Colour",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		ApplyStaticBind(std::make_unique<InputLayout>(gfx, input_element_desc, pvsbc));

		ApplyStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMatConst
		{
			float specular = .6f;
			float specularPow = 30.f;
			float padding[2];
		}colour_constant;

		ApplyStaticBind
		(
			std::make_unique<PixelConstantBuffer<PSMatConst>>(gfx, colour_constant, 1u)
		);

	}
	else
	{
		ApplyIndexFromStatic();
	}
	ApplyBind(std::make_unique<TransformConstBuffer>(gfx, *this));
}
