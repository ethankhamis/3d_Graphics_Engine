#pragma once
#include "winDef.h"
//#define NTDDI_VERSION
#include <d3d11.h>
#include "DirectXInfoManagement.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
#include "ExceptionHandler.h"
#include <wrl.h>
#include <vector>
#include "debugdefs.h"

namespace Bind
{
	struct Bindable;
}

struct Graphics
{
	friend Bind::Bindable;
public:
	Graphics( HWND hWnd, int w, int h);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
public:
	void EndFrame();
	void ClearBuffer(float R, float G, float B) noexcept;
public:
	void RenderIndexed(UINT count) noexcept_unless;
public:
	void ApplyProjection(DirectX::FXMMATRIX pj) noexcept;
	DirectX::XMMATRIX FetchProjection() const noexcept;
public:
	void SetCameraMat(DirectX::FXMMATRIX _camera) noexcept;
	DirectX::XMMATRIX FetchCameraMat() const noexcept;
private:
	DirectX::XMMATRIX camera;
public:// IMGUI RELATED
	void StartGUI() noexcept;
	void EndGUI() noexcept;
	bool GUI_Status() const noexcept;
private:// IMGUI RELATED
	bool Gui_Status = true;

private:
	DirectX::XMMATRIX projection;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
private:
#ifndef NDEBUG
	DxGfxInfoMng infomanager;
#endif // !NDEBUG
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