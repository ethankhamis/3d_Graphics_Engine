#pragma once
#include "isBinded.h"
struct Surface;
namespace Bind
{
	using Microsoft::WRL::ComPtr;

	struct Texture : public isBinded
	{
	public:
		Texture(Graphics& gfx, const std::wstring& pathway, UINT32 slot = NULL);
		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Texture> Store(Graphics& gfx, const std::wstring& pathway, UINT32 slot = NULL);
		static std::wstring ConstructUID(const std::wstring& pathway, UINT32 slot = NULL);
		std::wstring FetchUID() const noexcept override;
	protected:
		std::wstring pathway;
		ComPtr<ID3D11ShaderResourceView> pTextureView;
	private:
		UINT32 slot;
	};
}