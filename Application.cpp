#include "Application.h"
#include <sstream>
#include <iomanip>

void Application::ExecFrame()
{
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
