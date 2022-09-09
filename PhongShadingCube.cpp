#include "PhongShadingCube.h"
#include "DefaultBindables.h"
#include "Cube.h"
#include "ThrowMacros.h"

PhongShadingCube::PhongShadingCube(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 material)
	:
	TestObject( gfx, rng, adist, ddist, odist, rdist )
{
	if (!is_static_init())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 n;
		};
		auto model = Cube::Create_Independentf<Vertex>();
		model.ApplyNormalsIndependent();

		ApplyStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongShaderVS.cso");
		auto pvsbc = pvs->FetchByteCode();
		ApplyStaticBind(std::move(pvs));

		ApplyStaticBind(std::make_unique<PixelShader>(gfx, L"PhongShaderPS.cso"));

		ApplyStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		ApplyStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		ApplyStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		ApplyIndexFromStatic();
	}

	ApplyBind(std::make_unique<TransformConstBuffer>(gfx, *this));

	struct PSMaterialConst
	{
		DirectX::XMFLOAT3 colour;
		float specularIntensity = .6f;
		float specularPower = 10.f;
		float padding[3];
	} colorConst;
	colorConst.colour = material;
	ApplyBind(std::make_unique<PixelConstantBuffer<PSMaterialConst>>(gfx, colorConst, 1u));

	// model deformation transform (per instance, not stored as bind)
	DirectX::XMStoreFloat3x3(
		&model_transform,
		DirectX::XMMatrixScaling(2.0f, 2.0f, 2.3f)
	);
}


DirectX::XMMATRIX PhongShadingCube::FetchTransformMat() const noexcept
{
	return
		DirectX::XMLoadFloat3x3(&model_transform) *
		TestObject::FetchTransformMat();
}
