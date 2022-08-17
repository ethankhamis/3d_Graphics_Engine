#include "dxerr.h"
#include <sstream>
#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
#define GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HResultException( __LINE__,WFILE,hr )
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,WFILE,(hr) )



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
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.Windowed = 1;
	//Flipping Presentation
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = NULL;

	//use to check returns of d3d factory functions
	HRESULT hr;
	//create device + swapchain(front+back buffers) + rendering context

	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		NULL,
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
	GFX_THROW_FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
	GFX_THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTargetView));
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
	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED){throw
			GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());}
		else{GFX_THROW_FAILED(hr);}
	}
}


void Graphics::ClearBuffer(float R, float G, float B)
{
	const float colour[] = { R,G,B,1.0 };
	pDeviceContext->ClearRenderTargetView(pTargetView, colour);
}

Graphics::HResultException::HResultException(unsigned int line, const wchar_t* FName, HRESULT hr) noexcept
	:
	ExceptionHandler(line,FName),
	hr(hr)
{
}

const wchar_t* Graphics::HResultException::whatw() const noexcept
{

	std::wstringstream wss;
	wss
		<< FetchErrorType() << std::endl
		<< "[Error Code] 0x" << std::hex <<
		std::uppercase << FetchErrorCode()
		<< std::dec << " (" << 
		static_cast<unsigned long>(FetchErrorCode()) <<
		")" << std::endl
		<< "[Error String] " <<
		FetchErrorWString() << std::endl
		<< "[Description] " <<
		FetchErrorDescription() << std::endl
		<< FetchLine();
	buffer_w = wss.str();
	return buffer_w.c_str();
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
	DXGetErrorDescriptionW(hr, buffer, sizeof(buffer));
	return buffer;
}

const wchar_t* Graphics::DeviceRemovedException::FetchErrorType() const noexcept
{
	return L"Graphics Exception [Device_Removed] spec(DXGI_ERROR_DEVICE_REMOVED)";
}
