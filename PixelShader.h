#pragma once
#include "Bindable.h"
namespace Bind
{
	using Microsoft::WRL::ComPtr;

	struct PixelShader : public Bindable
	{
		PixelShader(Graphics& gfx, const std::wstring& filepath);
		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<PixelShader> Resolve(Graphics& gfx, const std::wstring& pathway);
		static std::wstring ConstructUID(const std::wstring& path);
		std::wstring FetchUID() const noexcept override;
	protected:
		ComPtr<ID3D11PixelShader> pPixelShader;
		std::wstring pathway;
	};
}