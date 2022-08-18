#include "Application.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Timer.h"
void Application::ExecFrame()
{
	const float colour = 0.5;
	window.grfx().ClearBuffer(1, 0.14, 0.1);
	window.grfx().RenderTestTriangle();
	window.grfx().EndFrame();
}

Application::Application()
:window(800, 500, L"6~3D"){}

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
