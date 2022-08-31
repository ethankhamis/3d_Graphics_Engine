#include "Application.h"
#include <sstream>
#include "Box.h"
#include <memory>
#include "Timer.h"
#include "MathematicalConstants.h"

Application::Application()
	:window(800, 600, L"6~3D") {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 1.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.08f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 400; i++)
	{
		boxes.emplace_back(std::make_unique<Box>(
			window.grfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	window.grfx().ApplyProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void Application::ExecFrame()
{
	auto delta = timer.MarkTime();
	window.grfx().ClearBuffer(1.0f, 1.0f, 1.0f);
	for (auto& box : boxes)
	{
		box->Update(delta);
		box->Render(window.grfx());
	}
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
