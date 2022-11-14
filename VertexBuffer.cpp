#include "VertexBuffer.h"
#include "BindableCodex.h"

namespace Bind
{
	VertexBuffer::VertexBuffer(Graphics& gfx, const DynamicVertex::VertexBuffer& vertexBuffer) : VertexBuffer(gfx, L"?", vertexBuffer)
	{}
	VertexBuffer::VertexBuffer(Graphics& gfx, const std::wstring& tag, const DynamicVertex::VertexBuffer& vertexBuffer)
		:
		stride(static_cast<uint32_t>(vertexBuffer.FetchLayout().size())),
		tag(tag)
	{
		DEF_INFOMANAGER(gfx);
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = UINT(vertexBuffer.size_bytes());
		bufferDesc.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA SubresourceData = {};
		SubresourceData.pSysMem = vertexBuffer.FetchData();
		GFX_THROW_INFO(FetchDevice(gfx)->CreateBuffer(&bufferDesc, &SubresourceData, &pVertexBuffer));
	}

	void VertexBuffer::Bind(Graphics& gfx) noexcept
	{
		const UINT offset = 0u;
		FetchDeviceContext(gfx)
			->
			IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}
	std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(Graphics& gfx, const std::wstring& tag, const DynamicVertex::VertexBuffer& vertexBuffer)
	{
		assert(tag != L"?");
		return Codex::Resolve<VertexBuffer>(gfx, tag, vertexBuffer);
	}
	std::wstring VertexBuffer::FetchUID() const noexcept
	{
		return ConstructUID(tag);
	}
	std::wstring VertexBuffer::ConstructUID_(const std::wstring& tag) 
	{
		using namespace std::string_literals;
		return convert::make_wstring(typeid(VertexBuffer).name()) + L"#"s + tag;
	}

}
