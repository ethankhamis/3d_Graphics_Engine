#include "Texture.h"
#include "Surface.h"
#include "DirectXInfoManagement.h"
#include "ThrowMacros.h"

Texture::Texture(Graphics& gfx, const Surface& surface)
{
	DEF_INFOMANAGER(gfx);
	D3D11_TEXTURE2D_DESC textureDescription = {};
	textureDescription.Width = surface.FetchWidth();
	textureDescription.Height = surface.FetchHeight();
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDescription.SampleDesc.Count = 1;
	textureDescription.SampleDesc.Quality = 0;
	textureDescription.Usage = D3D11_USAGE_DEFAULT;
	textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA srd = {};
	int memPitch = surface.FetchWidth() * sizeof(Colour);
	srd.pSysMem = surface.FetchpBuffer();
	srd.SysMemPitch = memPitch;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture;
	GFX_THROW_INFO(FetchDevice(gfx)->CreateTexture2D(&textureDescription, &srd, &Texture));


	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = textureDescription.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(FetchDevice(gfx)->CreateShaderResourceView(
		Texture.Get(), &srvd, &pTextureView
	));
}

void Texture::Bind(Graphics& gfx) noexcept
{
	FetchDeviceContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}