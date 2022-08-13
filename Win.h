#pragma once
#include "WinDef.h"

class Wnd
{
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
	Wnd(int width, int height, const wchar_t* name) noexcept;
	~Wnd() noexcept;
	Wnd(const Wnd&) = delete;
	Wnd& operator=(const Wnd&) = delete;
private:
	LRESULT MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; //CALLBACK -> stdcall
	static LRESULT CALLBACK HandleMsgBypass(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; //CALLBACK -> stdcall
};