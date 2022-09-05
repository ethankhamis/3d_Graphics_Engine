#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float rad)
:
	geo(gfx,rad),
	Constb(gfx)
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
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	constbuffer_data
		=
	{
		{ 0.0f,0.0f,0.0f }, 
		{ 0.7f,0.7f,0.9f }, 
		{ 0.05f,0.05f,0.05f }, 
		{ 1.0f,1.0f,1.0f }, 
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void PointLight::Render(Graphics& gfx) const noexcept(Debug)
{
	geo.SetPosition(constbuffer_data.pos);
	geo.Render(gfx);
}

void PointLight::Bind(Graphics& gfx) const noexcept
{
	Constb.Update(gfx, constbuffer_data);
	Constb.Bind(gfx);
}
