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
	public:
		static std::shared_ptr<VertexShader> Resolve(Graphics& gfx, const std::wstring& pathway);
		static std::wstring ConstructUID(const std::wstring& pathway);
		std::wstring FetchUID() const noexcept override;
	protected:
		WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		WRL::ComPtr<ID3DBlob> pByteCodeBlob;
		std::wstring pathway;
	};
}
