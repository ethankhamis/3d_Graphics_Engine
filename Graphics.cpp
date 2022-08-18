#include "dxerr.h"
#include <sstream>
#include <iostream>
#include <d3dcompiler.h>
#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)

//namespace wrl = Microsoft::WRL;
//
// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HResultException( __LINE__,WFILE,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HResultException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HResultException( __LINE__,WFILE,(hr),infomanager.FetchMessages() )
#define GFX_THROW_INFO(hrcall) infomanager.Apply(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,WFILE,(hr),infomanager.FetchMessages() )
#define GFX_THROW_INFO_ONLY(call) infomanager.Apply(); (call); {auto m = infomanager.FetchMessages(); if(!m.empty()) {throw Graphics::InfoException( __LINE__,WFILE,m);}}

#else
#define GFX_EXCEPT(hr) Graphics::HResultException( __LINE__,WFILE,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,WFILE,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
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
	SwapChainDesc.OutputWindow = hWnd;
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

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource),&pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTargetView));

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
	pDeviceContext->ClearRenderTargetView(pTargetView.Get(), colour);
}

void Graphics::RenderTestTriangle()
{
	namespace wrl = Microsoft::WRL;

	HRESULT hr;

	struct Vertex
	{
		float x;
		float y;
		float r;
		float g;
		float b;
	};

	// create vertex buffer (1 2d triangle at center of screen)
	const Vertex vertices[] =
	{
		{ 0.0f,0.5f,1.0f,0.0f,0.0f },
		{ 0.5f,-0.5f,0.0f,1.0f,0.0f },
		{ -0.5f,-0.5f,0.0f,0.0f,1.0f },
	};
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	// Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


	// create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// bind pixel shader
	pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	// create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// bind vertex shader
	pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


	// input (vertex) layout (2d position only)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_R32G32B32_FLOAT,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	GFX_THROW_INFO(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	// bind vertex layout
	pDeviceContext->IASetInputLayout(pInputLayout.Get());


	// bind render target
	pDeviceContext->OMSetRenderTargets(1u, pTargetView.GetAddressOf(), nullptr);


	// Set primitive topology to triangle list (groups of 3 vertices)
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1u, &vp);


	GFX_THROW_INFO_ONLY(pDeviceContext->Draw((UINT)std::size(vertices), 0u));

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
