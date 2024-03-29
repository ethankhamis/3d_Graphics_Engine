#include "Application.h"
#include <sstream>
#include <memory>
#include "Timer.h"
#include "Math.cpp"
#include <DirectXMath.h>
#include <algorithm>
#include "GfxDeviceInterface+Mng.h"
#include "Surface.h"
#include "imgui/imgui.h"
#include "DynamicVertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

GDIpManager gdipm;

Application::Application()
	:window(1280, 720, L"6~3D"),
	spawn(window.grfx())
	//plane(window.grfx(), 20.f, L"Models\\brickwall\\brickwall.jpg", L"Models\\brickwall\\brickwall_normal.jpg"),
{
	//set perspective projection 
	window.grfx().ApplyProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 300.0f));
}

void Application::ExecFrame()
{
	float delta = timer.MarkTime() * speed;
	window.grfx().ClearBuffer(.02f, 0.02f, .0f);
	window.grfx().SetCameraMat(camera.FetchMatrix());
	spawn.FetchLight()->Update(window.grfx(), camera.FetchMatrix());

	//building.Render(window.grfx());
	spawn.FetchLight()->Render(window.grfx());
	//wall.Render(window.grfx());
	model.Render(window.grfx());

	while (const std::optional<Keyboard::Event> key = window.kbd.Key_Read())
	{
		if (!key->Pressed())
		{
			continue;
		}
		switch(key->Keycode_Get())
		{
		case VK_ESCAPE:
			if (window.Cursor_Status())
			{
				window.Mouse_DisableIcon();
				window.mouse.Raw_Mouse_Enable();
			}
			else
			{
				window.Mouse_EnableIcon();
				window.mouse.Raw_Mouse_Disable();
			}break;
		}
	}
	if (!window.Cursor_Status())
	{
		if (window.kbd.Key_Pressed('W'))
		{
			camera.Translate_by({ 0.0f,0.0f,delta });
		}
		if (window.kbd.Key_Pressed('A'))
		{
			camera.Translate_by({ -delta,0.0f,0.0f });
		}
		if (window.kbd.Key_Pressed('S'))
		{
			camera.Translate_by({ 0.0f,0.0f,-delta });
		}
		if (window.kbd.Key_Pressed('D'))
		{
			camera.Translate_by({ delta,0.0f,0.0f });
		}
		if (window.kbd.Key_Pressed('R'))
		{
			camera.Translate_by({ 0.0f,delta,0.0f });
		}
		if (window.kbd.Key_Pressed('F'))
		{
			camera.Translate_by({ 0.0f,-delta,0.0f });
		}
	}
	while (const auto delta_raw = window.mouse.Access_Raw_Data())
	{
		if (!window.Cursor_Status())
		{
			camera.Rotate_by(static_cast<float>(delta_raw->x), static_cast<float>(delta_raw->y));
		}
	}


	camera.ConstructControlWindow();
	spawn.FetchLight()->ControlWnd();
	model.PresentWindow();

	//ImGui::End();
	
	window.grfx().EndFrame();
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
