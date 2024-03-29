#include "Texture.h"
#include "Surface.h"
#include "DirectXInfoManagement.h"
#include "ThrowMacros.h"
#include "isBinded_Storage.h"
namespace Bind
{
	Texture::Texture(Graphics& gfx, const std::wstring& pathway, UINT32 slot)
	:slot(slot), pathway(pathway) {
		DEF_INFOMANAGER(gfx);
		//create surface constructed using filename of image
		const Surface surface = Surface::WithFile(pathway);
		//create texture description
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

		//fill subresource data
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
		srvd.Texture2D.MostDetailedMip = NULL;
		srvd.Texture2D.MipLevels = 1;
		//create ahader resource view using device
		GFX_THROW_INFO(FetchDevice(gfx)->CreateShaderResourceView(
			Texture.Get(), &srvd, &pTextureView
		));
	}

	void Texture::Bind(Graphics& gfx) noexcept
	{
		//Bind the shader resources to the pixel shader stage of the pupeline
		FetchDeviceContext(gfx)->PSSetShaderResources(slot /*start slot*/, 1u, pTextureView.GetAddressOf()/*texture view*/);
	}
	std::shared_ptr<Texture> Texture::Store(Graphics& gfx, const std::wstring& pathway, UINT32 slot)
	{
		//pass the Texture to the depository
		return Repository::Store<Texture>(gfx, pathway, slot);
	}
	std::wstring Texture::ConstructUID(const std::wstring& pathway, UINT32 slot)
	{
		using namespace std::string_literals;
		//create UID using name, pathway and slot in combined form
		return convert::make_wstring(typeid(Texture).name()) + L"#"s + pathway + L"#" + convert::make_wstring(std::to_string(slot).c_str());
	}
	std::wstring Texture::FetchUID() const noexcept
	{
		return ConstructUID(pathway, slot);
	}
}