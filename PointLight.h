#pragma once

#include "Graphics.h"
#include "ConstBuffer(s).h"
#include "BindSolidLongLatSphere.h"

struct PointLight
{
	PointLight(Graphics& gfx, float rad = .5f);
	void ControlWnd() noexcept;
	void Reset() noexcept;
	void Render(Graphics& gfx) const noexcept (Debug);
	void Bind(Graphics& gfx) const noexcept;

private:
	struct PointLightConstantBuffer
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 materialColor;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightConstantBuffer constbuffer_data;
	mutable SolidLongLatSphere geo;
	mutable PixelConstantBuffer<PointLightConstantBuffer> Constb;
};