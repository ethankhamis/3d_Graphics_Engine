#pragma once
#include "WinDef.h"
#include "ExceptionHandler.h"
#include "Graphics.h"
#include <optional>
#include <memory>
#include "Keyboard.h"
#include "Mouse.h"

class Wnd
{
public:
	Graphics& grfx();
private:
	std::unique_ptr<Graphics> pGraphics;
public:
	Keyboard kbd;
	Mouse mouse;
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
public:
	void ApplyTitle(const std::wstring& t);
private:
	LRESULT MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT __stdcall HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; //CALLBACK -> stdcall
	static LRESULT __stdcall HandleMsgBypass(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept; //CALLBACK -> stdcall
public:
	static std::optional<int> Messages() noexcept;
public:




	struct Exception : public ExceptionHandler
	{
		static std::wstring ConvertErrorCode(HRESULT hResult) noexcept;
	};
	struct HResultException : public ExceptionHandler
	{
		HResultException(unsigned int curLine, const wchar_t* fName, HRESULT hResult) noexcept;
		const wchar_t* whatw() const noexcept override; //override std::ExceptionHandler's 'what()' func
		std::wstring FetchErrorDescription() const noexcept;
		/*virtual*/ const wchar_t* FetchErrorType() const noexcept override;
		HRESULT FetchErrorCode() const noexcept;
	private:
		HRESULT hResult;
	};
	struct NoGfxException : public ExceptionHandler
	{
		using ExceptionHandler::ExceptionHandler;
		const wchar_t* FetchErrorType() const noexcept override;
	};
};