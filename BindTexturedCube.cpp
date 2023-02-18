#pragma once
#include "BindTexturedCube.h"
#include "Cube.h"
#include "PrimaryTransformConstBuffer.h"
#include "imgui/imgui.h"
#include "DefaultBinds.h"

TexturedCube::TexturedCube(Graphics& gfx, float size, std::wstring texture, std::optional<std::wstring> normal)
{
	using namespace Bind;
	IndexedTriangleList cube = Cube::Create_Skinned_Independentf();
	cube.Transform(DirectX::XMMatrixScaling(size, size, size));
	const std::wstring geometry_tag = L"cube. " + convert::make_wstring(std::to_string(size).c_str());
	ApplyBind(Bind::VertexBuffer::Store(gfx, geometry_tag, cube.vertices));
	ApplyBind(Bind::IndexBuffer::Store(gfx, geometry_tag, cube.indices));
	ApplyBind(Bind::Texture::Store(gfx, texture));
	ApplyBind(Bind::Texture::Store(gfx, normal.value(), 1u));
	std::shared_ptr<Bind::VertexShader> pVertexShader = Bind::VertexShader::Store(gfx, L"PhongShaderVS.cso");
	auto pVertexShaderByteCode = pVertexShader->FetchByteCode();
	ApplyBind(std::move(pVertexShader));
	ApplyBind(Bind::PixelShader::Store(gfx, L"PhongShaderNormalPS.cso"));

	ApplyBind(Bind::PixelConstantBuffer<PixelShaderMaterialProperties>::Store(gfx, pixelshaderproperties, 1u));
	ApplyBind(Bind::InputLayout::Store(gfx, cube.vertices.FetchLayout(), pVertexShaderByteCode));
	ApplyBind(Bind::PrimTopology::Store(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	ApplyBind(std::make_shared<Bind::PrimaryTransformConstBuffer>(gfx, *this, NULLUNSIGNED, 2u));
}

void TexturedCube::ApplyPos(float3 pos) noexcept
{
	this->pos = {pos.x, pos.y, pos.z};
}

void TexturedCube::ApplyRot(float roll, float pitch, float yaw) noexcept
{
	this->orientation = { roll, pitch, yaw };
}

matrix TexturedCube::FetchTransformMat() const noexcept
{
	return matrix{
		DirectX::XMMatrixRotationRollPitchYaw(orientation.roll, orientation.pitch, orientation.yaw)
		*
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)
	};
}

void TexturedCube::Open_Window(Graphics& gfx) noexcept
{
	if (ImGui::Begin("Cube"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -90.0f, 90.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -90.0f, 90.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -90.0f, 90.0f, "%.1f");

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &orientation.roll, -180.f, 180.f);
		ImGui::SliderAngle("Pitch", &orientation.pitch, -180.f, 180.f);
		ImGui::SliderAngle("Yaw", &orientation.yaw, -180.f, 180.f);

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
