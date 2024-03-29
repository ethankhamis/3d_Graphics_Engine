#include "BindTexturedPlane.h"
#include "Plane.h"
#include "Converter.h"
#include "imgui/imgui.h"
#include "DefaultBinds.h"
#include "PrimaryTransformConstBuffer.h"
#include <optional>

TexturedPlane::TexturedPlane(Graphics& gfx, float size, std::wstring texture, std::optional<std::wstring> normal)
{
	IndexedTriangleList plane = Plane::Create_Textured();
	plane.Transform(DirectX::XMMatrixScaling(size, size, 1.f));
	const std::wstring geometry_tag = L"$plane. " + convert::make_wstring( std::to_string(size).c_str() );
	ApplyBind(Bind::VertexBuffer::Store(gfx, geometry_tag, plane.vertices));
	ApplyBind(Bind::IndexBuffer::Store(gfx, geometry_tag, plane.indices));
	ApplyBind(Bind::Texture::Store(gfx, texture));
	ApplyBind(Bind::Texture::Store(gfx, normal.value(), 1u));
	std::shared_ptr<Bind::VertexShader> pVertexShader = Bind::VertexShader::Store(gfx, L"PhongShaderVS.cso");
	auto pVertexShaderByteCode = pVertexShader->FetchByteCode();
	ApplyBind(std::move(pVertexShader));
	ApplyBind(Bind::PixelShader::Store(gfx, L"PhongShaderNormalPS.cso"));

	ApplyBind(Bind::PixelConstantBuffer<PixelShaderMaterialProperties>::Store(gfx, pixelshaderproperties, 1u));
	ApplyBind(Bind::InputLayout::Store(gfx, plane.vertices.FetchLayout(), pVertexShaderByteCode));
	ApplyBind(Bind::PrimTopology::Store(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	ApplyBind(std::make_shared<Bind::PrimaryTransformConstBuffer>(gfx, *this, NULLUNSIGNED, 2u));
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

void TexturedPlane::Open_Window(Graphics& gfx) noexcept
{
	if (ImGui::Begin("Plane"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &position.x, -90.0f, 90.0f, "%.1f");
		ImGui::SliderFloat("Y", &position.y, -90.0f, 90.0f, "%.1f");
		ImGui::SliderFloat("Z", &position.z, -90.0f, 90.0f, "%.1f");

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.f, 180.f);
		ImGui::SliderAngle("Pitch", &pitch, -180.f, 180.f);
		ImGui::SliderAngle("Yaw", &yaw, -180.f, 180.f);

		ImGui::Text("Shading Properties");
		bool intensity_ischanged =
			ImGui::SliderFloat("Specular Intensity", &pixelshaderproperties.specular_intensity, .0f, 1.f);
		bool power_ischanged =
			ImGui::SliderFloat("Specular Power", &pixelshaderproperties.specular_power, 0.f, 100.f);
		bool normal_state =
			pixelshaderproperties.normal_map_state == 1;
		bool normal_ischanged =
			ImGui::Checkbox("Enable Normal Map", &normal_state);

		pixelshaderproperties.normal_map_state = normal_state
			? 1 : NULL; // 1 = true null = false

		if (intensity_ischanged || normal_ischanged || power_ischanged)
		{
			QueryBindableObj<Bind::PixelConstantBuffer<PixelShaderMaterialProperties>>()->Update(gfx, pixelshaderproperties);
		}

	}
	ImGui::End();
}
