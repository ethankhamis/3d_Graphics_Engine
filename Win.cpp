#include "Win.h"
#include <sstream>
#include "ThrowMacros.h"
#include "Mouse.h"
#include "resource.h"
#include "imgui/imgui_impl_win32.h"

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
	wc.hInstance = hInstance; //(FetchInstance());
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

Graphics& Wnd::grfx()
{
	if (!pGraphics) { 
		throw WINDOW_NOGFX_EXCEPT(); }

	return *pGraphics;
}

void Wnd::Mouse_DisableIcon() noexcept
{
	Mouse_Icon_IsEnabled = false;
	Mouse_HideIcon();
	Mouse_HideIcon_GUI();
	Mouse_Icon_BlockInWindow();
}

void Wnd::Mouse_EnableIcon() noexcept
{
	Mouse_Icon_IsEnabled = true;
	Mouse_ShowIcon();
	Mouse_ShowIcon_GUI();
	Mouse_Icon_FreeRoam();
}

bool Wnd::Cursor_Status() const noexcept
{
	return Mouse_Icon_IsEnabled;
}

void Wnd::Mouse_HideIcon() noexcept
{
	while (::ShowCursor(0) >= 0);
}

void Wnd::Mouse_ShowIcon() noexcept
{
	while (::ShowCursor(1) < 0);
}

void Wnd::Mouse_ShowIcon_GUI() noexcept
{
	using namespace ImGui;
	GetIO().ConfigFlags&= ~ImGuiConfigFlags_NoMouse;
}

void Wnd::Mouse_HideIcon_GUI() noexcept
{
	using namespace ImGui;
	GetIO().ConfigFlags|= ImGuiConfigFlags_NoMouse;
}

void Wnd::Mouse_Icon_BlockInWindow() noexcept
{
	// find screen dimensions using hWnd+win functions
	RECT windowscreen;
	GetClientRect(hWnd, &windowscreen);
	MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&windowscreen), 2);
	ClipCursor(&windowscreen); // clip mouse to window
}

void Wnd::Mouse_Icon_FreeRoam() noexcept
{
	ClipCursor(nullptr);
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
		throw THROW_WINDOW_LAST_EXCEPTION();
	};
	 
	//initialise window CreateWindowExW

	hWnd = CreateWindowExW( CS_OWNDC,
		WndClass::FetchName() , name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WndClass::FetchInstance(), this
	);
	if (hWnd == nullptr) { 
		throw THROW_WINDOW_LAST_EXCEPTION(); }

	//present window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	
	//initalise win32 implementation of ImGUI
	ImGui_ImplWin32_Init(hWnd);

	//create graphics instance
	pGraphics = std::make_unique<Graphics>(hWnd,width,height);

	//register mouse input (raw) device directx11

	RAWINPUTDEVICE raw_input_device;
	raw_input_device.usUsagePage = 1;
	raw_input_device.dwFlags = 0;
	raw_input_device.usUsage = 2;
	raw_input_device.hwndTarget = nullptr;
	if (!RegisterRawInputDevices(&raw_input_device, 0x01, sizeof(raw_input_device))) // if not registering
	{
		throw THROW_WINDOW_LAST_EXCEPTION();
	}

}

Wnd::~Wnd() noexcept
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(hWnd);
}

void Wnd::ApplyTitle(const std::wstring& t)
{
	if(SetWindowTextW(hWnd, t.c_str()) == NULL)
	{
		throw THROW_WINDOW_LAST_EXCEPTION();
	}
}

LRESULT Wnd::MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return 1;
	}
	const ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KILLFOCUS: // if current window loses focus (i.e., new window pops up)
		kbd.State_Clear(); // clear current keystate
		break;

		//Start of Windows Kbd Messages

	case WM_ACTIVATE:
		OutputDebugStringW(L"activate called\n");
		if (!Mouse_Icon_IsEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				OutputDebugStringW(L"Activate is locking mouse using\nMouse_Icon_BlockInWindow()\n");
				Mouse_Icon_BlockInWindow();
				Mouse_HideIcon();
				break;
			}
			OutputDebugStringW(L"activate is NOT locking mouse. using\nMouse_Icon_FreeRoam()\n");
			Mouse_Icon_FreeRoam();
			Mouse_ShowIcon();
		}break;

	case WM_CHAR:
		if (io.WantCaptureKeyboard) { break; } // when imgui is using keyboard inputs
		kbd.Char_OnPress(static_cast<unsigned char>(wParam));//  add the keycode to the buffer
		break;

	case WM_KEYUP: //key release messages
	case WM_SYSKEYUP:
		if (io.WantCaptureKeyboard) { break; } // when imgui is using keyboard inputs
		kbd.Key_OnRelease(static_cast<unsigned char>(wParam));//  add the keycode to the buffer
		break;

	case WM_KEYDOWN: //key press messages
	case WM_SYSKEYDOWN:
		if (io.WantCaptureMouse) { break; }
		if (!(kbd.AutoRepeat_State || lParam & 0x40000000 /*bit 30*/)) {
			kbd.Key_onPress(static_cast<unsigned char>(wParam)); //  add the keycode to the buffer
		}
		break;
		//End of Windows Kbd Messages
		//Start of Windows Mouse Messages
	case WM_MOUSEMOVE:
	{
		if (!Mouse_Icon_IsEnabled) // if mouse icon is disabled
		{
			if (!mouse.Inside_Window_Check()) // and mouse is outside of window
			{
				SetCapture(hWnd); // capture input to the window
				mouse.Mouse_Inside(); // set the mouse boolean to be inside
				Mouse_HideIcon(); // hide the mouse icon
			}break;
		}
		SetForegroundWindow(hWnd); // capture all input from the window and activate it
		if (io.WantCaptureMouse) { break; }
		const POINTS pt = MAKEPOINTS(lParam); // find dimensions for the mouse position
		if(pt.y<height && pt.y >=0 && pt.x>=0 && pt.x < width)
		{ // check if the current mouse position is inside of the screen
			mouse.Mouse_Pos_Change(pt.x, pt.y);
			// if the boolean hasnt been updated
			if (!mouse.Inside_Window_Check())
			{
				//set the capture to the window
				SetCapture(hWnd);
				//flip the boolean to be true
				mouse.Mouse_Inside();
			}
		}
		//if the mouse position is outside of the screen
		else
		{
			//check if the left, right or middle buttons are being pressed
			//(wparam holds this data)
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
			{
				//if they are, change the mouse position
				mouse.Mouse_Pos_Change(pt.x, pt.y);
			}
			else
			{
				//if not, the window should not be in focus
				ReleaseCapture();
				//the boolean should state that the mouse is outside (false)
				mouse.Mouse_Outside();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (io.WantCaptureMouse) { break; }
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.Left_Pressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (io.WantCaptureMouse) { break; }
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.Right_Pressed(pt.x, pt.y);
		break;
	}

	case WM_MBUTTONDOWN:
	{
		if (io.WantCaptureMouse) { break; }
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.Middle_Mouse_Pressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		if (io.WantCaptureMouse) { break; }
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
		if (io.WantCaptureMouse) { break; }
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
		if (io.WantCaptureMouse) { break; }
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
		if (io.WantCaptureMouse) { break; }
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.Wheel_Delta(pt.x, pt.y, delta);
		break;
	}

	case WM_INPUT: //raw mouse input
	{
		if (!mouse.Raw_Mouse_Status())
		{
			break;
		}

		unsigned int current_capacity;
		// sizeof input data

		if (
			GetRawInputData
			(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				nullptr,
				&current_capacity,
				sizeof(RAWINPUTHEADER))
			== -1) 
		{
			break;
		}
		Raw_Buffer.resize(current_capacity);

		if (
			GetRawInputData
			(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				Raw_Buffer.data(),
				&current_capacity,
				sizeof(RAWINPUTHEADER)
			) != current_capacity
			)
		{
			break;
		}

		const RAWINPUT& raw_input = reinterpret_cast<const RAWINPUT&>(*Raw_Buffer.data());
		if (raw_input.header.dwType == RIM_TYPEMOUSE && (raw_input.data.mouse.lLastX != 0 || raw_input.data.mouse.lLastY != 0))
		{
			mouse.ApplyRawDelta(raw_input.data.mouse.lLastX, raw_input.data.mouse.lLastY);
		}break;
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

std::optional<int> Wnd::Messages() noexcept
{
	MSG msg;
	while ((PeekMessageW(&msg, nullptr, 0, 0,PM_REMOVE)) > 0)
	{
			switch (msg.message) // switch incase new return cases are implemented
			{
			case WM_QUIT: //must check if quitting as PeekMessage does not
				return static_cast<int>(msg.wParam);
				break;
			default:
				break;
		}

		TranslateMessage(&msg); // turn WM_KEYBOARD messages into WM_CHAR (if necessary)
		DispatchMessage(&msg);

	}
	return std::optional<int>();
}

std::wstring Wnd::HResultException::FetchErrorDescription() const noexcept
{
	return Exception::ConvertErrorCode(hResult);
}

const wchar_t* Wnd::HResultException::FetchErrorType() const noexcept
{
	return L"Window Exception";
}

HRESULT Wnd::HResultException::FetchErrorCode() const noexcept
{
	return hResult;
}

std::wstring Wnd::Exception::ConvertErrorCode(HRESULT hResult) noexcept
{
	wchar_t* pMsgBuffer = nullptr;
	// for windows to allocate memory for error string
	// also make pointer point to it
	const DWORD nMsgLen = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hResult,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pMsgBuffer),
		0, nullptr);
	if (nMsgLen == NULL) { return L"Unknown/Undefined error code"; }
	std::wstring errorString = pMsgBuffer;//copy the error string to std::wstring from windows buffer
	LocalFree(pMsgBuffer); //free windows buffer previously used
	return errorString;
}

const wchar_t* Wnd::NoGfxException::FetchErrorType() const noexcept
{
	return L"Window Exception [No Graphics Influence]";
}


Wnd::HResultException::HResultException(unsigned int curLine, const wchar_t* fName, HRESULT hResult) noexcept
	:
	ExceptionHandler(curLine, fName),
	hResult(hResult)
{
}

const wchar_t* Wnd::HResultException::whatw() const noexcept
{
	std::wstringstream wss;

	wss
		<< FetchErrorType()
		<< std::endl
		<< L"{Error Code of: "
		<< FetchErrorCode() << L" }"
		<< std::endl
		<< L"Error Description: "
		<< FetchErrorDescription()
		<< std::endl
		<< FetchErrorWString();

	buffer_w = wss.str().c_str();
	return buffer_w.c_str();

}