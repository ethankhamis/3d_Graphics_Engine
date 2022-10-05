#pragma once
#include "Bindable.h"
namespace Bind
{
	namespace WRL = Microsoft::WRL;

	struct VertexShader : public Bindable
	{
		VertexShader(Graphics& gfx, const std::wstring& filepath);
		void Bind(Graphics& gfx) noexcept override;
		ID3DBlob* FetchByteCode() const noexcept;
	protected:
		WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		WRL::ComPtr<ID3DBlob> pByteCodeBlob;
	};
}
