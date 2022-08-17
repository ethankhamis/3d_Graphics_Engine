#include "dxerr.h"
#include <sstream>
#include <iostream>
#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)


// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HResultException( __LINE__,WFILE,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HResultException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HResultException( __LINE__,WFILE,(hr),infomanager.FetchMessages() )
#define GFX_THROW_INFO(hrcall) infomanager.Apply(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,WFILE,(hr),infomanager.FetchMessages() )
#else
#define GFX_EXCEPT(hr) Graphics::HResultException( __LINE__,WFILE,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,WFILE,(hr) )
#endif



Graphics::Graphics(HWND hWnd)
{
	//create swapchain device

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	//buffer dimensions
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	//Format of Buffers pixels
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//Refresh Rate
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	//Scaling and Scanline Ordering
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // shouldnt require scaling
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//Quality and Count (SampleDesc) - AntiAliasing
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	//Buffer Usage and Count
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1; // for two buffers (front+back)
	//Window
	SwapChainDesc.OutputWindow = (HWND)421;
	SwapChainDesc.Windowed = 1;
	//Flipping Presentation
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = NULL;

	//use to check returns of d3d factory functions
	HRESULT hr;
	//create device + swapchain(front+back buffers) + rendering context

	UINT swapCreateFlags = NULLUNSIGNED;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pDeviceContext
		)
	);

	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTargetView));
	pBackBuffer->Release();

}

Graphics::~Graphics()
{
	if (!pDevice)
	{
		pDevice->Release();
	}
	if (!pSwapChain)
	{
		pSwapChain->Release();
	}
	if (!pDeviceContext)
	{
		pDeviceContext->Release();
	}
	if (!pTargetView)
	{
		pTargetView->Release();
	}
}

void Graphics::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	infomanager.Apply();
#endif
	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED){
			throw
			GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());}
		else{throw GFX_EXCEPT(hr);}
	}
}


void Graphics::ClearBuffer(float R, float G, float B)
{
	const float colour[] = { R,G,B,1.0 };
	pDeviceContext->ClearRenderTargetView(pTargetView, colour);
}

Graphics::HResultException::HResultException(unsigned int curLine, const wchar_t* fName, HRESULT hr, std::vector<std::string> infoMsg) noexcept
	:
	Exception(curLine,fName),
	hr(hr)
{
	// connect info messages with newlines into single string

	for (const auto& m : infoMsg)
	{
		info.append(m);
		info.push_back('\n');
		
	}
	// remove final newline
	if (!info.empty()){info.pop_back();}
}

const wchar_t* Graphics::HResultException::whatw() const noexcept
{

	std::wstringstream wss;

	std::wstring ErrorType = FetchErrorType();

	/*
	wss << FetchErrorType();
	wss << std::endl;
	wss << L"{Error Code} 0x";
	wss << std::hex;
	wss << std::uppercase;
	wss << FetchErrorCode();
	wss << std::dec;
	wss << L" (";
	wss << static_cast<unsigned long>(FetchErrorCode());
	wss << L")";
	wss << std::endl;
	wss << L"{Error String} ";
	wss << FetchErrorWString();
	wss << std::endl;
	wss << L"{Description} ";
	wss << FetchErrorDescription();
	wss << std::endl;
	wss << FetchLine();
	*/
	wss << ErrorType
		<<
		std::endl
		<< L"[Error Code] 0x" << std::hex <<
		std::uppercase << FetchErrorCode()
		<< std::dec << L" (" <<
		static_cast<unsigned long>(FetchErrorCode()) <<
		L")" << std::endl
		<< L"[Error String] " <<
		FetchErrorWString() << std::endl
		<< L"[Description] " <<
		FetchErrorDescription() << std::endl
		<< L"Exception Thrown in Line " << FetchLine() << std::endl;

	if (!info.empty()) {
		wss << L"\n{Error Info}\n" << FetchErrorInfo() << std::endl <<
			std::endl;
	}
	wss << L"Exception Thrown in Line " << FetchLine() << std::endl;
	wss << Exception::FetchFileName();

	buffer_w = wss.str();
	return buffer_w.c_str(); //buffer_w.c_str();
}

const wchar_t* Graphics::HResultException::FetchErrorType() const noexcept
{
	return L"Graphics Exception";
}

HRESULT Graphics::HResultException::FetchErrorCode() const noexcept
{
	return hr;
}

std::wstring Graphics::HResultException::FetchErrorWString() const noexcept
{
	return DXGetErrorStringW(hr);
}

std::wstring Graphics::HResultException::FetchErrorDescription() const noexcept
{
	const unsigned int bufferSize = 512;
	wchar_t buffer[bufferSize];
	DXGetErrorDescriptionW(hr, buffer, bufferSize);
	return buffer;
}

std::wstring Graphics::HResultException::FetchErrorInfo() const noexcept
{

	std::wstring wsTmp(info.begin(), info.end());

	std::wstring info = wsTmp;

	return info;
}

const wchar_t* Graphics::DeviceRemovedException::FetchErrorType() const noexcept
{
	return L"Graphics Exception [Device_Removed] spec(DXGI_ERROR_DEVICE_REMOVED)";
}
