#include "PointLight.h"
#include "imgui/imgui.h"
#include "MathematicalConstants.h"
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
		ImGui::Text("Position");
		ImGui::SliderFloat("X Coordinate", &constbuffer_data.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y Coordinate", &constbuffer_data.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z Coordinate", &constbuffer_data.pos.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Intensity + Colour");
		ImGui::SliderFloat("Intensity", &constbuffer_data.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Colour", &constbuffer_data.diffuseColour.x);
		ImGui::ColorEdit3("Ambience", &constbuffer_data.ambient.x);

		ImGui::Text("Falloff Parameters");
		ImGui::SliderFloat("Constant", &constbuffer_data.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &constbuffer_data.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &constbuffer_data.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	constbuffer_data = {
		{ 0.0f,0.0f,0.0f },
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void PointLight::Render(Graphics& gfx) const noexcept_unless
{
	mesh.SetPosition(constbuffer_data.pos);
	mesh.Render(gfx);
}

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept
{
	PointLight::PointLightCBuf constbuffer_data_Copy = constbuffer_data;
	const vec32_t pos = DirectX::XMLoadFloat3(&constbuffer_data.pos);
	DirectX::XMStoreFloat3(&constbuffer_data_Copy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(gfx, constbuffer_data_Copy);
	cbuf.Bind(gfx);
}
