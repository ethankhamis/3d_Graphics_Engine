#include <Windows.h>

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE: // close window
		PostQuitMessage(0); // exit code
		break;
	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:
		POINTS pt = MAKEPOINTS(lParam);

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int       nCmdShow)
{
	const auto pClassName = L"3D Physics Engine";
	// reg window class
	WNDCLASSEX winClass = { 0 };
	winClass.cbSize = sizeof(winClass);
	winClass.style = CS_OWNDC;
	winClass.lpfnWndProc = WinProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = hInstance;
	winClass.hIcon = nullptr;
	winClass.hCursor = nullptr;
	winClass.hbrBackground = nullptr;
	winClass.lpszMenuName = nullptr;
	winClass.lpszClassName = pClassName;
	winClass.hIconSm = nullptr;
	RegisterClassEx( &winClass );

	// create window instance

	HWND hWnd = CreateWindowEx(
		0, pClassName,
		L"3D Physics Engine",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		300, 300, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);
	//present window
	ShowWindow(hWnd, SW_SHOW);

	//message handling
	MSG msg;
	BOOL getResult;
	while ((getResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//while (true);

	switch (getResult)
	{
	case -1:
		return -1;
		break;
	default:
		return msg.wParam;
		break;
	}
}