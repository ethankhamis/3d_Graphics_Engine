#include "PointLight.h"
#include "imgui/imgui.h"
#include "Math.cpp"
PointLight::PointLight(Graphics& gfx, float rad)
:
	mesh(gfx,rad),
	cbuf(gfx)
{
	Reset();
}

void PointLight::ControlWnd() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::BeginTabBar("Positional Controls");
		ImGui::SliderFloat("X", &constbuffer_data.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &constbuffer_data.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &constbuffer_data.pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset Positional Controls"))
		{
			ResetPos();
		}
		ImGui::EndTabBar();
		ImGui::BeginTabBar("Lighting Controls");
		ImGui::SliderFloat("Intensity", &constbuffer_data.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Colour", &constbuffer_data.diffuseColour.x);
		ImGui::ColorEdit3("Ambience", &constbuffer_data.ambient.x);
		if (ImGui::Button("Reset Lighting Controls"))
		{
			ResetColour();
		}
		ImGui::EndTabBar();
		ImGui::BeginTabBar("Attenuation Controls");
		ImGui::SliderFloat("Constant", &constbuffer_data.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &constbuffer_data.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &constbuffer_data.attQuad, 0.0000001f, 10.0f, "%.7f", 10);
		if (ImGui::Button("Reset Attenuation"))
		{
			ResetAttenuation();
		}
		ImGui::EndTabBar();

		if (ImGui::Button("Reset All"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	ResetPos();
	ResetAttenuation();
	ResetColour();
}

void PointLight::ResetPos() noexcept
{
	constbuffer_data =
	{
		{2.f,15.f,-5.f,},
		constbuffer_data.ambient,
		constbuffer_data.diffuseColour,
		constbuffer_data.diffuseIntensity,
		constbuffer_data.attConst,
		constbuffer_data.attLin,
		constbuffer_data.attQuad
		
	};
}

void PointLight::ResetAttenuation() noexcept
{
	constbuffer_data =
	{
		constbuffer_data.pos,
		constbuffer_data.ambient,
		constbuffer_data.diffuseColour,
		constbuffer_data.diffuseIntensity,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void PointLight::ResetColour() noexcept
{
	constbuffer_data =
	{
		constbuffer_data.pos,
		{.05f,.05f,.05f},
		{1.f,1.f,1.f},
		{1.f},
		constbuffer_data.attConst,
		constbuffer_data.attLin,
		constbuffer_data.attQuad
	};
}

void PointLight::Render(Graphics& gfx) const noexcept_unless
{
	mesh.SetPosition(constbuffer_data.pos);
	mesh.Render(gfx);
}

void PointLight::Update(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept
{
	//create a copy of the current data of the pointlight
	PointLight::PointLightCBuf constbuffer_data_Copy = constbuffer_data;
	// retrieve the current position
	const vec32_t pos = DirectX::XMLoadFloat3(&constbuffer_data.pos);
	//store the current position with respect to the camera perspective
	DirectX::XMStoreFloat3(&constbuffer_data_Copy.pos, DirectX::XMVector3Transform(pos, view));
	//update the  constant buffer to the updated data
	cbuf.Update(gfx, constbuffer_data_Copy);
	//bind new data to the constant buffer
	cbuf.Bind(gfx);
}
