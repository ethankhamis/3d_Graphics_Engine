#pragma once
#include "Bindable.h"
#include "DynamicVertex.h"
#include "ThrowMacros.h"
#include <string>
using Microsoft::WRL::ComPtr;
namespace Bind
{

	struct VertexBuffer : public Bindable
	{
		VertexBuffer(Graphics& gfx, const std::wstring& tag, const DynamicVertex::VertexBuffer& vertexBuffer);
		VertexBuffer(Graphics& gfx, const DynamicVertex::VertexBuffer& vertexBuffer);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::wstring& tag, const DynamicVertex::VertexBuffer& vertexBuffer);
		template<typename...Remainder>
		static std::wstring ConstructUID(const std::wstring& tag, Remainder&&...remainder);
		std::wstring FetchUID() const noexcept override;
		static std::wstring ConstructUID_(const std::wstring& tag);
	protected:
		std::wstring tag;
		UINT stride;
		ComPtr<ID3D11Buffer> pVertexBuffer;
	};
	template<typename ...Remainder>
	inline std::wstring VertexBuffer::ConstructUID(const std::wstring& tag, Remainder && ...remainder)
	{
		return ConstructUID_(tag);
	}
}