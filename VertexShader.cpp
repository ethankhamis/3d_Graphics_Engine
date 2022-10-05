
#include "ThrowMacros.h"
#include "VertexShader.h"
namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::wstring& filepath)
	{
		DEF_INFOMANAGER(gfx);

		GFX_THROW_INFO(D3DReadFileToBlob(filepath.c_str(), &pByteCodeBlob));

		GFX_THROW_INFO(FetchDevice(gfx)->CreateVertexShader
		(
			pByteCodeBlob->GetBufferPointer(), pByteCodeBlob->GetBufferSize(),
			nullptr, &pVertexShader
		)
		);
	}

	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		FetchDeviceContext(gfx)
			->
			VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* VertexShader::FetchByteCode() const noexcept
	{
		return pByteCodeBlob.Get();
	}
}
