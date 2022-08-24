#pragma once
#include "Bindable.h"

using Microsoft::WRL::ComPtr;

struct PixelShader : public Bindable
{
	PixelShader(Graphics& gfx, const std::wstring& filepath);
	void Bind(Graphics& gfx) noexcept override;
protected:
	ComPtr<ID3D11PixelShader> pPixelShader;
};