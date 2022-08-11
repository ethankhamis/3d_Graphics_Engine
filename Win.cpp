#include "Win.h"

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
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = FetchName();
	wc.hIconSm = nullptr;
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

Wnd::Wnd(int width, int height, const wchar_t* name) noexcept
{
	//get window size using region size of user
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	
	AdjustWindowRect(&wr,
		WS_CAPTION |
		WS_MINIMIZEBOX |
		WS_SYSMENU |
		WS_MAXIMIZEBOX,
		FALSE);

	//initialise window

	hWnd = CreateWindowExW( CS_OWNDC,
		WndClass::FetchName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WndClass::FetchInstance(), this
	);
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
		// set the WinAPI-managed user data to store a pointer to the window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set WndProc to normal (non-setup) handler since setup is now complete
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Wnd::HandleMsgThunk));
		//send message to to wndClass MsgHandler
		return pWnd->MsgHandler(hWnd, msg, wParam, lParam);
	}
	// on an instance of getting a message prior to WM_NCCREATE, handle defaultly
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Wnd::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Wnd* const pWnd = reinterpret_cast<Wnd*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->MsgHandler(hWnd, msg, wParam, lParam);
}
