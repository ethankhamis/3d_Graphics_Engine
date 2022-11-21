#include "BindSolidLongLatSphere.h"
#include "DefaultBindables.h"
#include "ThrowMacros.h"
#include "LongLatSphere.h"
#include "VertexBuffer.h"


SolidLongLatSphere::SolidLongLatSphere(Graphics& gfx, float rad)
{
	using namespace Bind;

		auto model = Sphere::Create();
		model.Transform(DirectX::XMMatrixScaling(rad, rad, rad));

		const std::wstring Geometry_name = L"$sphere. " + convert::make_wstring(std::to_string(rad).c_str());

		ApplyBind(VertexBuffer::Resolve(gfx, Geometry_name, model.vertices));
		ApplyBind(IndexBuffer::Resolve(gfx, Geometry_name , model.indices));

		std::shared_ptr<Bind::VertexShader> pVertexShader = VertexShader::Resolve(gfx, L"SolidVS.cso");
		ID3DBlob* pvsbc = pVertexShader->FetchByteCode();
		ApplyBind(std::move(pVertexShader));
		ApplyBind(PixelShader::Resolve(gfx, L"SolidPS.cso"));
		struct PSColourConstant
		{
			DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;
		} colour_const;
		ApplyBind(PixelConstantBuffer<PSColourConstant>::Resolve(gfx, colour_const,1U));
		ApplyBind(InputLayout::Resolve(gfx, model.vertices.FetchLayout(), pvsbc));

		ApplyBind(PrimTopology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		ApplyBind(std::make_shared<TransformConstBuffer>(gfx, *this));
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
