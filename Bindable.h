#pragma once
#include "Graphics.h"

struct Bindable
{
	virtual void Bind(Graphics& gfx) noexcept = 0; // make abstract
	virtual ~Bindable() = default;

protected: //standard getter functions
	static ID3D11Device* FetchDevice(Graphics& gfx) noexcept;
	static ID3D11DeviceContext* FetchDeviceContext(Graphics& gfx) noexcept;
	static DxGfxInfoMng& FetchInfoManager(Graphics& gfx) noexcept(Debug);

};