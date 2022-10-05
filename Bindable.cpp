#include "Bindable.h"
#include <exception>

namespace Bind
{
	ID3D11Device* Bindable::FetchDevice(Graphics& gfx) noexcept
	{
		return gfx.pDevice.Get();
	}

	ID3D11DeviceContext* Bindable::FetchDeviceContext(Graphics& gfx) noexcept
	{
		return gfx.pDeviceContext.Get();
	}

	DxGfxInfoMng& Bindable::FetchInfoManager(Graphics& gfx) noexcept_unless
	{
#ifndef NDEBUG
		return gfx.infomanager;
#else
		throw std::logic_error("Tried to access gfx.infomanager within Release Mode");
#endif // !NDEBUG
	}
}