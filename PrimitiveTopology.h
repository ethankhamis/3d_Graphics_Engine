#pragma once
#include "Bindable.h"
namespace Bind
{
	struct PrimTopology : public Bindable
	{
		PrimTopology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY t);
		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<PrimTopology> Resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY primitive_type);
		static std::wstring ConstructUID(D3D11_PRIMITIVE_TOPOLOGY primitive_type);
		std::wstring FetchUID() const noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY topology;
	};
}