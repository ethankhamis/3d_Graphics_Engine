#pragma once
#include "winDef.h"
//#define NTDDI_VERSION
#include <d3d11.h>
#include "DirectXInfoManagement.h"
#include "ExceptionHandler.h"
#include <wrl.h>
#include <vector>

struct Graphics
{
	Graphics( HWND hWnd );
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
public:
	void EndFrame();
	void ClearBuffer(float R, float G, float B);
public:

private:
#ifndef NDEBUG
	DxGfxInfoMng infomanager;
#endif // !NDEBUG

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;

private:

public:
	class Exception : public ExceptionHandler { using ExceptionHandler::ExceptionHandler; };
	class HResultException : public Exception
	{
	public:   
		HResultException(unsigned int curLine, const wchar_t* fName, HRESULT hr, std::vector<std::string> infoMsg = {})  noexcept;
		const wchar_t* whatw() const noexcept; //override;
		const wchar_t* FetchErrorType() const noexcept override;
		HRESULT FetchErrorCode() const noexcept;
		std::wstring FetchErrorWString() const noexcept;
		std::wstring FetchErrorDescription() const noexcept;
		std::wstring FetchErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class DeviceRemovedException : public HResultException
	{
		using HResultException::HResultException;
	public:
		const wchar_t* FetchErrorType() const noexcept override;
	private:
		std::wstring reason;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(unsigned int curLine, const wchar_t* fName, std::vector<std::string> infoMsgs) noexcept;
		const wchar_t* whatw() const noexcept override;
		const wchar_t* FetchErrorType() const noexcept override;
		std::wstring FetchErrorInfo() const noexcept;
	private:
		std::string info;
	
	};
};