#pragma once
#include "winDef.h"
#include <d3d11.h>
#include "ExceptionHandler.h"

struct Graphics
{
	Graphics( HWND hWnd );
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
public:
	void EndFrame();
	void ClearBuffer(float R, float G, float B);
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11RenderTargetView* pTargetView = nullptr;
private:

public:
	//class Exception : ExceptionHandler { using ExceptionHandler::ExceptionHandler; };
	class HResultException : public ExceptionHandler
	{
	public:
		HResultException(unsigned int line, const wchar_t* FName, HRESULT HResult)  noexcept;
		const wchar_t* whatw() const noexcept override;
		const wchar_t* FetchErrorType() const noexcept override;
		HRESULT FetchErrorCode() const noexcept;
		std::wstring FetchErrorWString() const noexcept;
		std::wstring FetchErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class DeviceRemovedException : public HResultException
	{
		using HResultException::HResultException;
		const wchar_t* FetchErrorType() const noexcept override;
	};
};