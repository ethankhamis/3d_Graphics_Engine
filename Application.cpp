#include "Application.h"
#include <sstream>
#include <memory>
#include "Timer.h"
#include "MathematicalConstants.h"
#include <DirectXMath.h>
#include <algorithm>
#include "GfxDeviceInterface+Mng.h"
#include "Surface.h"
//#include "Spawn.h"
#include "imgui/imgui.h"
#include "DynamicVertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


GDIpManager gdipm;

Application::Application()
	:window(1000, 750, L"6~3D"),
	spawn(window.grfx())
{
	window.grfx().ApplyProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void Application::ExecFrame()
{
	float delta = timer.MarkTime() * speed;

	window.grfx().ClearBuffer(0.01f, 0.0f, 0.0f);
	window.grfx().SetCameraMat(camera.FetchMatrix());
	spawn.FetchLight()->Bind(window.grfx(), camera.FetchMatrix());

	const matrix transform = DirectX::XMMatrixRotationRollPitchYaw
	(
		pos.roll, pos.pitch, pos.yaw
	)
		*
	DirectX::XMMatrixTranslation(pos.x,pos.y,pos.z);

	building.Render(window.grfx(),transform);

	spawn.FetchLight()->Render(window.grfx());

	camera.ConstructControlWindow();
	spawn.FetchLight()->ControlWnd();
	PresentModelWnd();
	//spawn.Window(drawables);

	//ImGui::End();
	
	window.grfx().EndFrame();
}

void Application::PresentModelWnd()
{
	if (ImGui::Begin("Model"))
	{
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);
	}
	ImGui::End();
}


Application::~Application() {}

int Application::Start()
{
	while (1)
	{
		if (const std::optional<int>(exceptionCode) = Wnd::Messages())
		{
			return *exceptionCode;
		}

		ExecFrame();
	}
}
