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

	//message handling
	MSG msg;
	BOOL getResult;
	while ((getResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg); // turn WM_KEYBOARD messages into WM_CHAR (if necessary)
		DispatchMessage(&msg);
		ExecFrame();
	}
	switch (getResult) // incase new return cases are implemented
	{
	case -1:
		return -1;
		break;
	default:
		return msg.wParam;
		break;
	}
}
