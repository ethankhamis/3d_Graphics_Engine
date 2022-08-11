
#include "Win.h"


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int       nCmdShow)
{
	// create window instance
	Wnd wnd(800, 300, L"6~3D");
	//message handling
	MSG msg;
	BOOL getResult;
	while ((getResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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