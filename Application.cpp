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
	

	//Spawn spawn(window.grfx());
	//drawables.reserve(nDrawables);
	//std::generate_n(std::back_inserter(drawables), nDrawables, Spawn{ window.grfx() });
	
	for (int i = 0; i < nDrawables; i++)
	{
		drawables.emplace_back(spawn.Random());
	}
	
	window.grfx().ApplyProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));

}

void Application::ExecFrame()
{
	auto delta = timer.MarkTime() * speed;

	window.grfx().ClearBuffer(0.01f, 0.0f, 0.0f);

	window.grfx().SetCameraMat(camera.FetchMatrix());
	for (auto& drawable : drawables)
	{
		drawable->Update(window.kbd.Key_Pressed(VK_SPACE) ? 0.0f : delta);
		drawable->Render(window.grfx());
	}

	spawn.Window(drawables);

	if (ImGui::Begin("Time"))
	{
		ImGui::SliderFloat("Scale Speed", &speed, 0.f, 4.f);
	}

	ImGui::End();
	camera.ConstructControlWindow();
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
