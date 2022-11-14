#include "PrimitiveTopology.h"
#include "BindableCodex.h"

namespace Bind
{
	PrimTopology::PrimTopology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY ty)
		:topology(ty) {}

	void PrimTopology::Bind(Graphics& gfx) noexcept
	{
		FetchDeviceContext(gfx)
			->
			IASetPrimitiveTopology(topology);
	}
	std::shared_ptr<PrimTopology> PrimTopology::Resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY primitive_type)
	{
		return Codex::Resolve<PrimTopology>(gfx, primitive_type);
	}
	std::wstring PrimTopology::ConstructUID(D3D11_PRIMITIVE_TOPOLOGY primitive_type)
	{
		using namespace std::string_literals;
		return convert::make_wstring(typeid(PrimTopology).name()) + L"#"s + convert::make_wstring(std::to_string(primitive_type).c_str());
	}
	std::wstring PrimTopology::FetchUID() const noexcept
	{
		return ConstructUID(topology);
	}
}
