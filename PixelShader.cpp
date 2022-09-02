#include "PixelShader.h"
#include "ThrowMacros.h"

namespace WRL = Microsoft::WRL;

PixelShader::PixelShader(Graphics& gfx, const std::wstring& filepath)
{
	DEF_INFOMANAGER(gfx);
	ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(filepath.c_str(), &pBlob));
	GFX_THROW_INFO
	(
		FetchDevice(gfx)
		->
		CreatePixelShader(pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(), nullptr, &pPixelShader)
	);
}

void PixelShader::Bind(Graphics& gfx) noexcept
{
	FetchDeviceContext(gfx)
		->
		PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
