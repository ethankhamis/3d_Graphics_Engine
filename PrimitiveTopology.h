#pragma once
#include "Bindable.h"
namespace Bind
{
	struct PrimTopology : public Bindable
	{
		PrimTopology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY t);
		void Bind(Graphics& gfx) noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY topology;
	};
}