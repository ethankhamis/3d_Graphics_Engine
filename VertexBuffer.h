#pragma once
#include "isBinded.h"
#include "DynamicVertex.h"
#include "ThrowMacros.h"
#include <string>
using Microsoft::WRL::ComPtr;
namespace Bind
{

	struct VertexBuffer : public isBinded
	{
		VertexBuffer(Graphics& gfx, const std::wstring& tag, const DynamicVertex::VertexBuffer& vertexBuffer);
		VertexBuffer(Graphics& gfx, const DynamicVertex::VertexBuffer& vertexBuffer);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<VertexBuffer> Store(Graphics& gfx, const std::wstring& tag, const DynamicVertex::VertexBuffer& vertexBuffer);
		template<typename...Remainder>
		static std::wstring ConstructUID(const std::wstring& tag, Remainder&&...remainder); // takes the remainder out of ConstructUID_
		std::wstring FetchUID() const noexcept override;
		static std::wstring ConstructUID_(const std::wstring& tag);
	protected:
		std::wstring tag;
		UINT stride;
		ComPtr<ID3D11Buffer> pVertexBuffer; //comptr -> shared_ptr for COM objects
	};
	template<typename ...Remainder>
	inline std::wstring VertexBuffer::ConstructUID(const std::wstring& tag, Remainder && ...remainder)
	{ //use of plural type to store other arguments
		// this allows for the tag to only be taken
		return ConstructUID_(tag);
	}
}