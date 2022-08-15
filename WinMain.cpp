#include <algorithm>
#include <iterator>
#include "Win.h"


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int       nCmdShow)
{
	try {
		// create window instance
		Wnd wnd(800, 500, L"6~3D");
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
	catch (const ExceptionHandler& exception)
	{
		MessageBoxW(nullptr, exception.whatw(), exception.FetchErrorType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& exception)
	{
		// Convert e.what() to a wide string
		std::string  cs(exception.what());
		std::wstring ws;
		copy(cs.begin(), cs.end(), back_inserter(ws));

		MessageBoxW(nullptr, ws.c_str()
			, L"std Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxW(nullptr, L"No details", L"Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}