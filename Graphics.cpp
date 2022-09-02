#include "dxerr.h"
#include <sstream>
#include <iostream>
#include <d3dcompiler.h>
#include "Graphics.h"
#include "ThrowMacros.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


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

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//use to check returns of d3d factory functions
	HRESULT hr;
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

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource),&pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTargetView));

	// create depth stensil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	// bind depth state
	pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stensil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 1000u;
	descDepth.Height = 750u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &pDepthStencilView
	));

	// bind depth stensil view to OM
	pDeviceContext->OMSetRenderTargets(1u, pTargetView.GetAddressOf(), pDepthStencilView.Get());

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 1000.0f;
	vp.Height = 750.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pDeviceContext->RSSetViewports(1u, &vp);

	//initialise d3d implementation of IMGUI
	ImGui_ImplDX11_Init(pDevice.Get(), pDeviceContext.Get());
}

void Graphics::EndFrame()
{
	if (Gui_Status)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
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


void Graphics::ClearBuffer(float R, float G, float B) noexcept
{
	if (Gui_Status)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float colour[] = { R,G,B,1.0f };
	pDeviceContext->ClearRenderTargetView(pTargetView.Get(), colour);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}


void Graphics::DrawIndexed(UINT count) noexcept(!Debug)
{
	GFX_THROW_INFO_ONLY(pDeviceContext->DrawIndexed(count, 0u, 0u));
}

void Graphics::ApplyProjection(DirectX::FXMMATRIX pj) noexcept
{
	projection = pj;
}

DirectX::XMMATRIX Graphics::FetchProjection() const noexcept
{
	return projection;
}

void Graphics::StartGUI() noexcept
{
	Gui_Status = true;
}

void Graphics::EndGUI() noexcept
{
	Gui_Status = false;
}

bool Graphics::GUI_Status() const noexcept
{
	return Gui_Status;
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

Graphics::InfoException::InfoException(unsigned int curLine, const wchar_t* fName, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(curLine,fName)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info.append(m);
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const wchar_t* Graphics::InfoException::whatw() const noexcept
{
	std::wstringstream wss;
	wss << FetchErrorType() << std::endl
		<< "\n[Error Info]\n" << FetchErrorInfo() 
		<< std::endl << std::endl << FetchErrorWString();
	buffer_w = wss.str();
	return buffer_w.c_str();
}

const wchar_t* Graphics::InfoException::FetchErrorType() const noexcept
{
	return L"Graphics Exception";
}

std::wstring Graphics::InfoException::FetchErrorInfo() const noexcept
{
	std::wstring wsTmp(info.begin(), info.end());

	std::wstring info = wsTmp;

	return info;
}
