#pragma once
#include "Bindable.h"

using Microsoft::WRL::ComPtr;

struct Sampler : public Bindable
{
	Sampler(Graphics& gfx);
	void Bind(Graphics& gfx) noexcept override;
protected:
	ComPtr<ID3D11SamplerState> pSampler;
};