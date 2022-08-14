#include "Win.h"
#include <sstream>
#include "resource.h"

Wnd::WndClass Wnd::WndClass::wndClass;

Wnd::WndClass::WndClass() noexcept
	:
	hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = FetchInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,16,16,0 ));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = FetchName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassExW(&wc);
}

Wnd::WndClass::~WndClass() noexcept
{
	UnregisterClassW(wndClassName, FetchInstance());
}

const wchar_t* Wnd::WndClass::FetchName() noexcept
{
	return wndClassName;
}

HINSTANCE Wnd::WndClass::FetchInstance() noexcept
{
	return wndClass.hInstance;
}

Wnd::Wnd(int width, int height, const wchar_t* name)
{
	//get window size using region size of user
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	
	if ( FAILED(AdjustWindowRect(&wr,
		WS_CAPTION |
		WS_MINIMIZEBOX |
		WS_SYSMENU |
		WS_MAXIMIZEBOX,
		FALSE)))
	{
		throw CHWND_LAST_EXCEPT();
	};
	 
	//initialise window CreateWindowExW

	hWnd = CreateWindowExW( CS_OWNDC,
		WndClass::FetchName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WndClass::FetchInstance(), this
	);
	if (hWnd == nullptr) { throw CHWND_LAST_EXCEPT(); }

	//present window
	ShowWindow(hWnd, SW_SHOWDEFAULT);

}

Wnd::~Wnd() noexcept
{
	DestroyWindow(hWnd);
}

LRESULT Wnd::MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Wnd::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// acquire pointer to window class from Non-Client creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Wnd* const pWnd = static_cast<Wnd*>(pCreate->lpCreateParams);
		// store ptr to a window class with WinApi
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set WndProc to normal (non-setup) handler since setup is now complete
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Wnd::HandleMsgBypass));
		//send message to to wndClass MsgHandler
		return pWnd->MsgHandler(hWnd, msg, wParam, lParam);
	}
	// on an instance of getting a message prior to WM_NCCREATE, handle defaultly
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Wnd::HandleMsgBypass(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Wnd* const pWnd = reinterpret_cast<Wnd*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->MsgHandler(hWnd, msg, wParam, lParam);
}

Wnd::Exception::Exception(unsigned int curLine, const char* fName, HRESULT hResult) noexcept
	:
	ExceptionHandler(curLine, fName),
	hResult(hResult)
{
}

const char* Wnd::Exception::what() const noexcept
{
	std::ostringstream oss;

	oss
		<< FetchErrorType()
		<< std::endl
		<< "{Error Code of: " <<
		FetchErrorCode() <<   " }"
		<< "{Error Description: " <<
		FetchErrorString() << " }"
		<< FetchStartString();

	buffer_w = oss.str().c_str();
	return oss.str().c_str();

}

std::string Wnd::Exception::FetchErrorString() const noexcept
{
	return ConvertErrorCode(hResult);
}

const char* Wnd::Exception::FetchErrorType() const noexcept
{
	return "Window Exception";
}

HRESULT Wnd::Exception::FetchErrorCode() const noexcept
{
	return hResult;
}

std::string Wnd::Exception::ConvertErrorCode(HRESULT hResult) noexcept
{
	char* pMsgBuffer = nullptr;
	// for windows to allocate memory for error string
	// also make pointer point to it
	DWORD nMsgLen = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pMsgBuffer),
		0, nullptr);
	if (nMsgLen == NULL) { return "Unknown/Undefined error code"; }
	std::string errorString = pMsgBuffer;//copy the error string to std::string from windows buffer
	LocalFree(pMsgBuffer); //free windows buffer previously used
	return errorString;
}
