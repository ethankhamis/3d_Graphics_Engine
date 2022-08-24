#pragma once
#include "Bindable.h"

namespace WRL = Microsoft::WRL;
struct InputLayout : public Bindable
{
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderByteCode);
	void Bind(Graphics& gfx) noexcept override;
protected:
	WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};