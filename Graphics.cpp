
#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

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

	//create device + swapchain(front+back buffers) + rendering context

	D3D11CreateDeviceAndSwapChain
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
		);

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
}

void Graphics::EndFrame()
{
	pSwapChain->Present( 1, NULL );
}
