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
#include "DynamicVertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


GDIpManager gdipm;

Application::Application()
	:window(1000, 750, L"6~3D"),
	spawn(window.grfx())
{
	//for (int i = 0; i < nDrawables; i++)
	{
		//drawables.emplace_back(spawn.Chosen(5));
	}
	for (std::unique_ptr<Drawable>& pDrawable : drawables)
	{
		if (PhongShadingCube* pPhongShadedCube = dynamic_cast<PhongShadingCube*>(pDrawable.get()))
		{
			cubes.emplace_back(pPhongShadedCube);
		}
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


	SpawnSimulationWindow();
	camera.ConstructControlWindow();
	spawn.FetchLight()->ControlWnd();
	spawn.Window(drawables);
	SpawnCubeWindowManagerWindow();
	SpawnCubeWindows();

	ImGui::End();
	
	window.grfx().EndFrame();
}

void Application::SpawnSimulationWindow() noexcept
{
	if (ImGui::Begin("Time"))
	{
		ImGui::SliderFloat("Scale Speed", &speed, 0.f, 10.f, "%.4f", 3.2f);
	}

	if (window.kbd.Key_Pressed(VK_DELETE))
	{
		drawables.clear();
	}
}

void Application::SpawnCubeWindowManagerWindow() noexcept
{
	// imgui window to open box windows
	if (ImGui::Begin("Boxes"))
	{
		using namespace std::string_literals;
		const auto preview = comboCubeIndex ? std::to_string(*comboCubeIndex) : "Choose a box..."s;
		if (ImGui::BeginCombo("Box Number", preview.c_str()))
		{
			comboCubeIndex = 0;
			for (int i = 0; i < cubes.size(); i++)
			{
				const bool selected = *comboCubeIndex == i;
				if (ImGui::Selectable(std::to_string(i).c_str(), selected))
				{
					comboCubeIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Spawn Control Window") && comboCubeIndex)
		{
			cubeControlIds.insert(*comboCubeIndex);
			comboCubeIndex.reset();
		}
	}
	ImGui::End();
}

void Application::SpawnCubeWindows()
{
	for (auto i = cubeControlIds.begin(); i != cubeControlIds.end(); )
	{
		if (!cubes[*i]->ControlWnd(*i, window.grfx()))
		{
			i = cubeControlIds.erase(i);
		}
		else
		{
			++i;
		}
	}
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
