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
		DirectX::XMFLOAT3 position;
		float padding;
	};

	DirectX::XMFLOAT3 position = { .0f,.0f,.0f };
	mutable SolidLongLatSphere geo;
	mutable PixelConstantBuffer<PointLightConstantBuffer> Constb;
};