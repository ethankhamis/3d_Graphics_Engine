#pragma once
#include "winDef.h"
#include <d3d11.h>

struct Graphics
{
	Graphics( HWND hWnd );
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	void FillScreen();
	void ClearBuffer(float R, float G, float B);
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;
	ID3D11RenderTargetView* pTargetView = nullptr;

};