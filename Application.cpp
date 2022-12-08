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


GDIpManager gdipm;

Application::Application()
	:window(1280, 720, L"6~3D"),
	spawn(window.grfx()),
	plane(window.grfx(), 15.f,L"Models\\brickwall\\brickwall.jpg")
{
	plane.ApplyPos({ 1.f,-5.f,10.f });
	window.grfx().ApplyProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
	//window.Mouse_DisableIcon();
}

void Application::ExecFrame()
{
	float delta = timer.MarkTime() * speed;

	window.grfx().ClearBuffer(1.0f, 0.0f, 0.0f);
	window.grfx().SetCameraMat(camera.FetchMatrix());
	spawn.FetchLight()->Bind(window.grfx(), camera.FetchMatrix());

	building.Render(window.grfx());
	building2.Render(window.grfx());
	spawn.FetchLight()->Render(window.grfx());
	plane.Render(window.grfx());

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

	building.PresentWindow("Model 1");
	building2.PresentWindow("Model 2");
	plane.Open_Window(window.grfx());


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
