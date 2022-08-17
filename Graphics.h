#pragma once
#include "winDef.h"
#include <d3d11.h>
#include "DirectXInfoManagement.h"
#include "ExceptionHandler.h"
#include <vector>

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
#ifndef NDEBUG
	DxGfxInfoMng infomanager;
#endif // !NDEBUG

	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11RenderTargetView* pTargetView = nullptr;
private:

public:
	class Exception : public ExceptionHandler { using ExceptionHandler::ExceptionHandler; };
	class HResultException : public Exception
	{
	public:   
		HResultException(unsigned int curLine, const wchar_t* fName, HRESULT hr, std::vector<std::wstring> infoMsg = {})  noexcept;
		const wchar_t* whatw() const noexcept; //override;
		const wchar_t* FetchErrorType() const noexcept override;
		HRESULT FetchErrorCode() const noexcept;
		std::wstring FetchErrorWString() const noexcept;
		std::wstring FetchErrorDescription() const noexcept;
		std::wstring FetchErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::wstring info;
	};
	class DeviceRemovedException : public HResultException
	{
		using HResultException::HResultException;
	public:
		const wchar_t* FetchErrorType() const noexcept override;
	private:
		std::wstring reason;
	};
};