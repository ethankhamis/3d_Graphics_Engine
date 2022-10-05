#include "BindSolidLongLatSphere.h"
#include "DefaultBindables.h"
#include "ThrowMacros.h"
#include "LongLatSphere.h"


SolidLongLatSphere::SolidLongLatSphere(Graphics& gfx, float rad)
{
	using namespace Bind;
	if (!is_static_init())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
		};

		auto model = Sphere::Create<Vertex>();
		model.Transform(DirectX::XMMatrixScaling(rad, rad, rad));
		ApplyBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
		ApplyIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"SolidVS.cso");
		auto pvsbc = pvs->FetchByteCode();
		ApplyStaticBind(std::move(pvs));
		ApplyStaticBind(std::make_unique<PixelShader>(gfx, L"SolidPS.cso"));
		struct PSColourConstant
		{
			DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;
		} colour_const;
		ApplyStaticBind(std::make_unique<PixelConstantBuffer<PSColourConstant>>(gfx, colour_const));
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		ApplyStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		ApplyStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else{ApplyIndexFromStatic();}

	ApplyBind(std::make_unique<TransformConstBuffer>(gfx, *this));
}

void SolidLongLatSphere::Update(float dt) noexcept
{

}

DirectX::XMMATRIX SolidLongLatSphere::FetchTransformMat() const noexcept
{
	return
		DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

void SolidLongLatSphere::SetPosition(DirectX::XMFLOAT3 position)
{
	this->position = position;
}
