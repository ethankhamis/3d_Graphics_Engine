#include "Sampler.h"
#include "ThrowMacros.h"

Sampler::Sampler(Graphics& gfx)
{
	DEF_INFOMANAGER(gfx);

	D3D11_SAMPLER_DESC samplerDescription = {};
	samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO(FetchDevice(gfx)->CreateSamplerState(&samplerDescription, &pSampler));
}

void Sampler::Bind(Graphics& gfx) noexcept
{
	FetchDeviceContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}
