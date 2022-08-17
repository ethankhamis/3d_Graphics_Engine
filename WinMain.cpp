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
	{							// error is specifically related to whatw() in graphics.cpp

		std::wstring s = exception.whatw();

		for (int i = 0;; i++)
		{
			if (s[i] == L'\0')
			{
				if (std::next(s[i]) != NULL)
				{
					s[i] = ' ';
					continue;
				}
			}
		}
		s += L'\0';
		
		

		MessageBoxExW(nullptr, s.c_str(), exception.FetchErrorType(), MB_OK | MB_ICONEXCLAMATION, NULL);
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