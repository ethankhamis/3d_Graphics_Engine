#include "PrimitiveTopology.h"
#include "isBinded_Storage.h"

namespace Bind
{
	PrimTopology::PrimTopology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY ty)
		:topology(ty) {}

	void PrimTopology::Bind(Graphics& gfx) noexcept
	{
	//set primitive topology to the input assembly
		FetchDeviceContext(gfx)
			->
			IASetPrimitiveTopology(topology);
	}
	std::shared_ptr<PrimTopology> PrimTopology::Store(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY primitive_type)
	{
		//store the primitive topology to the depository
		return Repository::Store<PrimTopology>(gfx, primitive_type);
	}
	std::wstring PrimTopology::ConstructUID(D3D11_PRIMITIVE_TOPOLOGY primitive_type)
	{
		//create UID from the ID of nmame and the primitive type 
		using namespace std::string_literals;
		return convert::make_wstring(typeid(PrimTopology).name()) + L"#"s + convert::make_wstring(std::to_string(primitive_type).c_str());
	}
	std::wstring PrimTopology::FetchUID() const noexcept
	{
		return ConstructUID(topology);
	}
}
