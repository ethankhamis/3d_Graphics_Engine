#pragma once
#include "isBinded.h"
namespace Bind
{
	using Microsoft::WRL::ComPtr;

	struct Sampler : public isBinded
	{
		Sampler(Graphics& gfx);
		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Sampler> Store(Graphics& gfx);
		static std::wstring ConstructUID();
		std::wstring FetchUID() const noexcept override;
	protected:
		ComPtr<ID3D11SamplerState> pSampler;
	};
}