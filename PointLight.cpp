#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float rad)
:
	geo(gfx,rad),
	Constb(gfx)
{
}

void PointLight::ControlWnd() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X Coordinate", &position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y Coordinate", &position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z Coordinate", &position.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	position = { .0f, .0f, .0f };
}

void PointLight::Render(Graphics& gfx) const noexcept(Debug)
{
	geo.SetPosition(position);
	geo.Render(gfx);
}

void PointLight::Bind(Graphics& gfx) const noexcept
{
	Constb.Update(gfx, PointLightConstantBuffer{ position });
}
