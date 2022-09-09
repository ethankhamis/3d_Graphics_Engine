#include "Application.h"
#include <sstream>
#include <memory>
#include "Timer.h"
#include "MathematicalConstants.h"
#include <DirectXMath.h>
#include <algorithm>
#include "GfxDeviceInterface+Mng.h"
#include "Surface.h"

#include "Spawn.h"

#include "imgui/imgui.h"


GDIpManager gdipm;

Application::Application()
	:window(1000, 750, L"6~3D"),
	spawn(window.grfx())
{
	for (int i = 0; i < nDrawables; i++)
	{
		drawables.emplace_back(spawn.Chosen(5));
	}
	
	window.grfx().ApplyProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));

}

void Application::ExecFrame()
{
	auto delta = timer.MarkTime() * speed;

	window.grfx().ClearBuffer(0.01f, 0.0f, 0.0f);
	window.grfx().SetCameraMat(camera.FetchMatrix());

	spawn.FetchLight()->Bind(window.grfx(), camera.FetchMatrix());

	for (auto& drawable : drawables)
	{
		drawable->Update(window.kbd.Key_Pressed(VK_SPACE) ? 0.0f : delta);
		drawable->Render(window.grfx());
	}
	spawn.FetchLight()->Render(window.grfx());


	if (ImGui::Begin("Time"))
	{
		ImGui::SliderFloat("Scale Speed", &speed, 0.f,10.f, "%.4f", 3.2f);
	}

	if (window.kbd.Key_Pressed(VK_DELETE))
	{
		drawables.clear();
	}

	ImGui::End();
	camera.ConstructControlWindow();
	spawn.FetchLight()->ControlWnd();
	spawn.Window(drawables);

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
