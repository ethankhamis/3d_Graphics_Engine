
#include "ThrowMacros.h"
#include "VertexShader.h"
#include "Converter.h"
#include "BindableCodex.h"
#include <typeinfo>

namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::wstring& filepath) : pathway(filepath)
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
	std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::wstring& pathway)
	{
		return Codex::Resolve<VertexShader>(gfx, pathway);
	}
	std::wstring VertexShader::ConstructUID(const std::wstring& pathway)
	{
		using namespace std::string_literals;
		return convert::make_wstring(typeid(VertexShader).name()) + L"#"s + pathway;
	}
	std::wstring VertexShader::FetchUID() const noexcept
	{
		return ConstructUID(pathway);
	}
}
