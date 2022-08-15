#include "Win.h"
#include <sstream>
#include "Mouse.h"
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

Wnd::Wnd(int width, int height, const wchar_t* name):width(width),height(height)
{
	//get window size using region size of user
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	
	if ( (AdjustWindowRect(&wr,
		WS_CAPTION |
		WS_MINIMIZEBOX |
		WS_SYSMENU |
		WS_MAXIMIZEBOX,
		FALSE)) == NULL)
	{
		throw EHWND_LAST_EXCEPT();
	};
	 
	//initialise window CreateWindowExW

	hWnd = CreateWindowExW( CS_OWNDC,
		WndClass::FetchName() , name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WndClass::FetchInstance(), this
	);
	if (hWnd == nullptr) { throw EHWND_LAST_EXCEPT(); }

	//present window
	ShowWindow(hWnd, SW_SHOWDEFAULT);

}

Wnd::~Wnd() noexcept
{
	DestroyWindow(hWnd);
}

void Wnd::ApplyTitle(const std::wstring& t)
{
	if(SetWindowTextW(hWnd, t.c_str()) == NULL)
	{
		throw EHWND_LAST_EXCEPT();
	}
}

LRESULT Wnd::MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KILLFOCUS: // if current window loses focus (i.e., new window pops up)
		kbd.State_Clear(); // clear current keystate
		break;

		//Start of Windows Kbd Messages

	case WM_CHAR:
		kbd.Char_OnPress(static_cast<unsigned char>(wParam));
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.Key_OnRelease(static_cast<unsigned char>(wParam));
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(kbd.AutoRepeat_State || lParam & 0x40000000 /*bit 30*/)) {
			kbd.Key_onPress(static_cast<unsigned char>(wParam));
		}
		break;


		//End of Windows Kbd Messages

		//Start of Windows Mouse Messages

	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if(pt.y<height && pt.y >=0 && pt.x>=0 && pt.x < width)
		{
			mouse.Mouse_Pos_Change(pt.x, pt.y);
			if (!mouse.Inside_Window_Check())
			{
				SetCapture(hWnd);
				mouse.Mouse_Inside();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
			{
				mouse.Mouse_Pos_Change(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.Mouse_Outside();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.Left_Pressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.Right_Pressed(pt.x, pt.y);
		break;
	}

	case WM_MBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.Middle_Mouse_Pressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.Left_Released(pt.x, pt.y);

		// release when outside of window
		if (pt.x >= width || pt.x < 0 || pt.y >= height || pt.y < 0)
		{
			ReleaseCapture();mouse.Mouse_Outside();
		}

		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.Right_Released(pt.x, pt.y);

		// release when outside of window
		if ( pt.x >= width || pt.x < 0 ||  pt.y >= height || pt.y < 0)
		{
			ReleaseCapture();mouse.Mouse_Outside();
		}
		break;
	}

	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.Middle_Mouse_Released(pt.x, pt.y);

		// release if outside of window
		if (pt.x >= width || pt.x < 0 || pt.y >= height || pt.y < 0)
		{
			ReleaseCapture();mouse.Mouse_Outside();
		}
		break;
	}

	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.Wheel_Delta(pt.x, pt.y, delta);
		break;
	}
	//End of Windows Mouse Messages


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

std::optional<int> Wnd::Messages()
{
	MSG msg;
	while ((GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		if (msg.message == WM_QUIT)
		{
			switch (msg.message) // switch incase new return cases are implemented
			{
			case WM_QUIT:
				return msg.wParam;
				break;
			default:
				break;
			}
		}

		TranslateMessage(&msg); // turn WM_KEYBOARD messages into WM_CHAR (if necessary)
		DispatchMessage(&msg);

	}
	return std::optional<int>();
}

Wnd::Exception::Exception(unsigned int curLine, const wchar_t* fName, HRESULT hResult) noexcept
	:
	ExceptionHandler(curLine, fName),
	hResult(hResult)
{
}

const wchar_t* Wnd::Exception::whatw() const noexcept
{
	std::wstringstream wss;

	wss
		<< FetchErrorType()
		<< std::endl
		<< L"{Error Code of: "
		<< FetchErrorCode() <<   L" }"
		<< std::endl
		<< L"{Error Description: "
		<< FetchErrorString() << L" }"
		<< FetchStartString();

	buffer_w = wss.str().c_str();
	return buffer_w.c_str();

}

std::wstring Wnd::Exception::FetchErrorString() const noexcept
{
	return ConvertErrorCode(hResult);
}

const wchar_t* Wnd::Exception::FetchErrorType() const noexcept
{
	return L"Window Exception";
}

HRESULT Wnd::Exception::FetchErrorCode() const noexcept
{
	return hResult;
}

std::wstring Wnd::Exception::ConvertErrorCode(HRESULT hResult) noexcept
{
	wchar_t* pMsgBuffer = nullptr;
	// for windows to allocate memory for error string
	// also make pointer point to it
	DWORD nMsgLen = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pMsgBuffer),
		0, nullptr);
	if (nMsgLen == NULL) { return L"Unknown/Undefined error code"; }
	std::wstring errorString = pMsgBuffer;//copy the error string to std::wstring from windows buffer
	LocalFree(pMsgBuffer); //free windows buffer previously used
	return errorString;
}
