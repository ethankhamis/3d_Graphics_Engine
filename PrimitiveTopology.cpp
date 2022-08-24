#include "PrimitiveTopology.h"
#pragma once

PrimTopology::PrimTopology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY ty)
:topology(ty){}

void PrimTopology::Bind(Graphics& gfx) noexcept
{
	FetchDeviceContext(gfx)
		->
		IASetPrimitiveTopology(topology); // t = type;
}
