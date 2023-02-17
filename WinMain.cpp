#include <algorithm>
#include <iterator>
#include "Win.h"
#include "Application.h"
#include "Converter.h"

//WinMain -> Entry Point
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int       nCmdShow)
{
	try
	{
		//stores the message handler and game loop
		return Application{}.Start();
	}
	//catch custom exceptions, std::exceptions and unidentified exceptiosn
	catch (const ExceptionHandler& exception)
	{	
		MessageBoxW(nullptr, exception.whatw(), exception.FetchErrorType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& exception)
	{
		MessageBoxExW(nullptr, convert::make_wstring(exception.what()).c_str()
			, L"std Exception", MB_OK | MB_ICONEXCLAMATION,NULL);
	}
	catch (...)
	{
		MessageBoxExW(nullptr, L"No details", L"Unknown exception", MB_OK | MB_ICONEXCLAMATION,NULL);
	}
	return -1;
}