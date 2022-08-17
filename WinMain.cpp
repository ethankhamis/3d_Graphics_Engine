#include <algorithm>
#include <iterator>
#include "Win.h"
#include "Application.h"


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int       nCmdShow)
{
	try
	{
		return Application{}.Start();
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

		MessageBoxExW(nullptr, ws.c_str()
			, L"std Exception", MB_OK | MB_ICONEXCLAMATION,NULL);
	}
	catch (...)
	{
		MessageBoxExW(nullptr, L"No details", L"Unknown exception", MB_OK | MB_ICONEXCLAMATION,NULL);
	}
	return -1;
}