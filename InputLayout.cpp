#include "InputLayout.h"
#include "ThrowMacros.h"
#pragma once
namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
	{
		DEF_INFOMANAGER(gfx);

		GFX_THROW_INFO(FetchDevice(gfx)->CreateInputLayout
		(
			layout.data(), static_cast<UINT>(layout.size()),
			pVertexShaderByteCode->GetBufferPointer(),
			pVertexShaderByteCode->GetBufferSize(),
			&pInputLayout
		)
		);
	}

	void InputLayout::Bind(Graphics& gfx) noexcept
	{
		FetchDeviceContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}
}