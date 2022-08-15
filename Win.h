#pragma once
#include "WinDef.h"
#include "ExceptionHandler.h"
#include "Keyboard.h"

class Wnd
{
public:
	Keyboard kbd;
private:
	unsigned int width;
	unsigned int height;
	HWND hWnd;
private:
	// for cleanup management and registering window
	class WndClass
	{
	private:
		WndClass() noexcept;
		~WndClass() noexcept;
		WndClass(const WndClass&) = delete;
		WndClass& operator=(const WndClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"6~3D";
		static WndClass wndClass;
		HINSTANCE hInstance;
	public:
		static const wchar_t* FetchName() noexcept; //getter for className
		static HINSTANCE FetchInstance() noexcept; //getter for HInstance
	};
public:
	Wnd(int width, int height, const wchar_t* name);
	~Wnd() noexcept;
	Wnd(const Wnd&) = delete;
	Wnd& operator=(const Wnd&) = delete;
private:
	LRESULT MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; //CALLBACK -> stdcall
	static LRESULT CALLBACK HandleMsgBypass(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; //CALLBACK -> stdcall
public:
	class Exception : public ExceptionHandler
	{
	public:
		Exception(unsigned int curLine, const wchar_t* fName, HRESULT hResult) noexcept;
		const wchar_t* whatw() const noexcept override; //override std::ExceptionHandler's 'what()' func
		std::wstring FetchErrorString() const noexcept;
		virtual const wchar_t* FetchErrorType() const noexcept;
		HRESULT FetchErrorCode() const noexcept;
		static std::wstring ConvertErrorCode(HRESULT hResult) noexcept;
	private:
		HRESULT hResult;
	};
};

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)

#define CHWND_EXCEPT( hr ) Wnd::Exception( __LINE__,WFILE,hr )
#define CHWND_LAST_EXCEPT() Wnd::Exception( __LINE__,WFILE,GetLastError() )