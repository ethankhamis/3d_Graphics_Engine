#pragma once
#include "Bindable.h"
struct Surface;
namespace Bind
{
	using Microsoft::WRL::ComPtr;

	struct Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const Surface& surface, UINT32 slot = NULL);
		void Bind(Graphics& gfx) noexcept override;
	protected:
		ComPtr<ID3D11ShaderResourceView> pTextureView;
	private:
		UINT32 slot;
	};
}