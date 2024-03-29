#pragma once
#include "Graphics.h"
#include "debugdefs.h"
namespace Bind
{
	struct isBinded
	{
		virtual void Bind(Graphics& gfx) noexcept = 0; // make abstract
		virtual ~isBinded() = default;
		virtual std::wstring FetchUID() const noexcept //function should never be called
		{
			assert(false); 
			return L"";
		}

	protected: //standard getter functions
		static ID3D11Device* FetchDevice(Graphics& gfx) noexcept;
		static ID3D11DeviceContext* FetchDeviceContext(Graphics& gfx) noexcept;
		static DxGfxInfoMng& FetchInfoManager(Graphics& gfx);

	};
}