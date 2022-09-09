#pragma once
#include "Graphics.h"
#include "BindSolidLongLatSphere.h"
#include "ConstBuffer(s).h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void ControlWnd() noexcept;
	void Reset() noexcept;
	void Render(Graphics& gfx) const noexcept(!Debug);
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColour;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf constbuffer_data;
	mutable SolidLongLatSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};