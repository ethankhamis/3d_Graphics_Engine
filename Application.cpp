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


	building.Render(window.grfx());

	spawn.FetchLight()->Render(window.grfx());

	camera.ConstructControlWindow();
	spawn.FetchLight()->ControlWnd();
	PresentDemoWindow();
	building.PresentWindow();
	//spawn.Window(drawables);

	//ImGui::End();
	
	window.grfx().EndFrame();
}

void Application::PresentDemoWindow()
{
	static bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
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
