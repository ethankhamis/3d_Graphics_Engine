#pragma once
#include "isBinded.h"
namespace Bind
{
	struct PrimTopology : public isBinded
	{
		PrimTopology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY t);
		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<PrimTopology> Store(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY primitive_type);
		static std::wstring ConstructUID(D3D11_PRIMITIVE_TOPOLOGY primitive_type);
		std::wstring FetchUID() const noexcept override;
	protected:
		D3D11_PRIMITIVE_TOPOLOGY topology;
	};
}

