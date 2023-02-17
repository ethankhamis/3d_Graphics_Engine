#include "isBinded.h"
#include <exception>

namespace Bind
{
	ID3D11Device* isBinded::FetchDevice(Graphics& gfx) noexcept
	{
		return gfx.pDevice.Get();
	}

	ID3D11DeviceContext* isBinded::FetchDeviceContext(Graphics& gfx) noexcept
	{
		return gfx.pDeviceContext.Get();
	}

	DxGfxInfoMng& isBinded::FetchInfoManager(Graphics& gfx)
	{
#ifndef NDEBUG
		return gfx.infomanager;
#else
		throw std::logic_error("Tried to access gfx.infomanager within Release Mode");
#endif // !NDEBUG
	}
}