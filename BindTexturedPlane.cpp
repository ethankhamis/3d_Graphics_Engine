#include "BindTexturedPlane.h"
#include "Plane.h"
#include "Converter.h"
#include "DefaultBindables.h"

TexturedPlane::TexturedPlane(Graphics& gfx, float size, std::wstring texture)
{
	IndexedTriangleList plane = Plane::Create_Textured();
	plane.Transform(DirectX::XMMatrixScaling(size, size, 1.f));
	const std::wstring geometry_tag = L"$plane. " + convert::make_wstring( std::to_string(size).c_str() );
	ApplyBind(Bind::VertexBuffer::Resolve(gfx, geometry_tag, plane.vertices));
	ApplyBind(Bind::IndexBuffer::Resolve(gfx, geometry_tag, plane.indices));
	ApplyBind(Bind::Texture::Resolve(gfx, texture));
	std::shared_ptr<Bind::VertexShader> pVertexShader = Bind::VertexShader::Resolve(gfx, L"PhongShaderVS.cso");
	auto pVertexShaderByteCode = pVertexShader->FetchByteCode();
	ApplyBind(std::move(pVertexShader));
	ApplyBind(Bind::PixelShader::Resolve(gfx, L"PhongShaderPS.cso"));
	
	struct PixelShaderMaterialConstant
	{
		float specular_intensity = 0.1f,
			specualar_power = 20.f,
			padding[2];
	}pixelmatconst;

	ApplyBind(Bind::PixelConstantBuffer<PixelShaderMaterialConstant>::Resolve(gfx, pixelmatconst, 1u));
	ApplyBind(Bind::InputLayout::Resolve(gfx, plane.vertices.FetchLayout(), pVertexShaderByteCode));
	ApplyBind(Bind::PrimTopology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	ApplyBind(std::make_shared<Bind::TransformConstBuffer>(gfx, *this));
}

void TexturedPlane::ApplyPos(float3 pos) noexcept
{
	position = pos;
}

void TexturedPlane::ApplyRot(float roll, float pitch, float yaw) noexcept
{
	this->roll = roll;
	this->pitch = pitch;
	this->yaw = yaw;
}

matrix TexturedPlane::FetchTransformMat() const noexcept
{
	using namespace DirectX;
	return XMMatrixRotationRollPitchYaw(roll, pitch, yaw)  *  XMMatrixTranslation(position.x, position.y, position.z);
}
