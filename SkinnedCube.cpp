#include "SkinnedCube.h"
#include "DefaultBindables.h"
#include "ThrowMacros.h"
#include "Cube.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"

SkinnedCube::SkinnedCube(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist)
	:
	TestObject(gfx,rng,adist,ddist,odist,rdist)
{

	if (!is_static_init())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 tc;
		};
		IndexedTriangleList<Vertex> model = Cube::Create_Skinned_Independentf<Vertex>();
		model.ApplyNormalsIndependent();
		ApplyStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		ApplyStaticBind(std::make_unique<Texture>(gfx, Surface::WithFile(L"Images\\eren.png")));
		ApplyStaticBind(std::make_unique<Sampler>(gfx));
		auto pvs = std::make_unique<VertexShader>(gfx, L"TexturedPhongShaderVS.cso");
		auto pvsbc = pvs->FetchByteCode();
		ApplyStaticBind(std::move(pvs));

		ApplyStaticBind(std::make_unique<PixelShader>(gfx, L"TexturedPhongShaderPS.cso"));

		ApplyStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA},
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		ApplyStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		ApplyStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMatConst
		{
			float specular_intensity = .6f;
			float specularPow = 30.f;
			float padding[2];
		}colour_constant;
		ApplyStaticBind(std::make_unique<PixelConstantBuffer<PSMatConst>>(gfx, colour_constant,1u));

	}
	else
	{
		ApplyIndexFromStatic();
	}

	ApplyBind(std::make_unique<TransformConstBuffer>(gfx, *this));

}