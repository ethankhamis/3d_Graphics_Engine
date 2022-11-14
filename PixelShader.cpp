#include "PixelShader.h"
#include "ThrowMacros.h"
#include "BindableCodex.h"
namespace Bind
{
	namespace WRL = Microsoft::WRL;

	PixelShader::PixelShader(Graphics& gfx, const std::wstring& filepath) : pathway(filepath)
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
	std::shared_ptr<PixelShader> PixelShader::Resolve(Graphics& gfx, const std::wstring& pathway)
	{
		return Codex::Resolve<PixelShader>(gfx, pathway);
	}
	std::wstring PixelShader::ConstructUID(const std::wstring& path)
	{
		using namespace std::string_literals;
		return convert::make_wstring(typeid(PixelShader).name()) + L"#"s + path;
	}
	std::wstring PixelShader::FetchUID() const noexcept
	{
		return ConstructUID(pathway);
	}
}