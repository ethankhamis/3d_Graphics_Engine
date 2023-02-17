#include "InputLayout.h"
#include "ThrowMacros.h"
#include "isBinded_Storage.h"
#include "DynamicVertex.h"
#pragma once
namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx, DynamicVertex::VertexLayout layout_input, ID3DBlob* pVertexShaderByteCode) : layout(std::move(layout_input))
	{
		DEF_INFOMANAGER(gfx);

		const std::vector<D3D11_INPUT_ELEMENT_DESC> d3dlayout = layout.FetchD3DLayout();

		//create input layout and allocate its appropriate memory
		GFX_THROW_INFO(FetchDevice(gfx)->CreateInputLayout
		(
			d3dlayout.data(), static_cast<UINT>(d3dlayout.size()),
			pVertexShaderByteCode->GetBufferPointer(),
			pVertexShaderByteCode->GetBufferSize(),
			&pInputLayout
		)
		);
	}

	void InputLayout::Bind(Graphics& gfx) noexcept
	{
		//bind input layout to the input assembler stage
		FetchDeviceContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}
	std::shared_ptr<InputLayout> InputLayout::Store(Graphics& gfx, const DynamicVertex::VertexLayout& layout, ID3DBlob* pByteCode_VS)
	{
		return Repository::Store<InputLayout>(gfx, layout, pByteCode_VS);
	}
	std::wstring InputLayout::ConstructUID(const DynamicVertex::VertexLayout& layout, ID3DBlob* pByteCode_VS)
	{
		using namespace std::string_literals;
		return convert::make_wstring( typeid(InputLayout).name() ) + L"#"s + layout.FetchID().c_str();
	}
	std::wstring InputLayout::FetchUID() const noexcept
	{
		return ConstructUID(layout);
	}
}