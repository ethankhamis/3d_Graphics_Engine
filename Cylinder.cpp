#include "Cylinder.h"
#include "Prism.h"
#include "DefaultBindables.h"

Cylinder::Cylinder(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist, std::uniform_int_distribution<int>& tdist)
	:
	TestObject(gfx, rng, adist, ddist, odist, rdist)
{
	if (!is_static_init())
	{

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongShaderVS.cso");
		auto pvsbc = pvs->FetchByteCode();
		ApplyStaticBind(std::move(pvs));

		ApplyStaticBind(std::make_unique<PixelShader>(gfx, L"IndexedPhongPS.cso"));


		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		ApplyStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		ApplyStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3A colors[6] = {
				{1.0f,0.0f,0.0f},
				{1.0f,0.0f,0.0f},
				{1.0f,0.0f,0.0f},
				{1.0f,0.0f,0.0f},
				{1.0f,0.0f,0.0f},
				{1.0f,0.0f,0.0f},
			};
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
		} matConst;
		ApplyStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, matConst, 1u));
	}
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};

	const IndexedTriangleList<Vertex> model = Prism::Create_IndependentNormals<Vertex>(tdist(rng));
	ApplyBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
	ApplyIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	ApplyBind(std::make_unique<TransformConstBuffer>(gfx, *this));
	
}