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
	:window(1920, 1080, L"6~3D"),
	spawn(window.grfx())
{
	window.grfx().ApplyProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
	//window.Mouse_DisableIcon();
}

void Application::ExecFrame()
{
	float delta = timer.MarkTime() * speed;

	window.grfx().ClearBuffer(0.01f, 0.0f, 0.0f);
	window.grfx().SetCameraMat(camera.FetchMatrix());
	spawn.FetchLight()->Bind(window.grfx(), camera.FetchMatrix());


	building.Render(window.grfx());
	spawn.FetchLight()->Render(window.grfx());


	while (const auto e = window.kbd.Key_Read())
	{
		if (e->Pressed() && e->Keycode_Get() == VK_INSERT)
		{
			if (window.Cursor_Status())
			{
				window.Mouse_DisableIcon();
				window.mouse.Raw_Mouse_Enable();
			}
			else
			{
				window.Mouse_EnableIcon();
				window.mouse.Raw_Mouse_Disable();
			}
		}
	}


	camera.ConstructControlWindow();
	spawn.FetchLight()->ControlWnd();
	building.PresentWindow();
	PresentRawInputWindow();
	//spawn.Window(drawables);

	//ImGui::End();
	
	window.grfx().EndFrame();
}

void Application::PresentRawInputWindow()
{
	while (const auto delta = window.mouse.Access_Raw_Data())
	{
		x += delta->x;
		y += delta->y;
	}
	using namespace ImGui;
	if (ImGui::Begin("Raw Input"))
	{
		ImGui::Text("Tally: (%d,%d)", x, y);
		ImGui::Text("Cursor: %s", window.Cursor_Status() ? "enabled" : "disabled");
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
