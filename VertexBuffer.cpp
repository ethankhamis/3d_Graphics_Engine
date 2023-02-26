#include "VertexBuffer.h"
#include "isBinded_Storage.h"

namespace Bind
{
	VertexBuffer::VertexBuffer(Graphics& gfx, const DynamicVertex::VertexBuffer& vertexBuffer) : VertexBuffer(gfx, L"?", vertexBuffer)
	{}
	VertexBuffer::VertexBuffer(Graphics& gfx, const std::wstring& tag, const DynamicVertex::VertexBuffer& vertexBuffer)
		:
		pitch(static_cast<uint32_t>(vertexBuffer.FetchLayout().size())),
		tag(tag)
	{
		DEF_INFOMANAGER(gfx);
		//create buffer description and fill its properties
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = UINT(vertexBuffer.size_bytes());
		bufferDesc.StructureByteStride = pitch;
		// fill subresource data
		D3D11_SUBRESOURCE_DATA SubresourceData = {};
		SubresourceData.pSysMem = vertexBuffer.FetchData();
		//use data to create vertex buffer
		GFX_THROW_INFO(FetchDevice(gfx)->CreateBuffer(&bufferDesc, &SubresourceData, &pVertexBuffer));
	}

	void VertexBuffer::Bind(Graphics& gfx) noexcept
	{
		const UINT offset = 0u;
		//set the vertex buffers within the input assembler
		FetchDeviceContext(gfx)
			->
			IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &pitch, &offset);
	}
	std::shared_ptr<VertexBuffer> VertexBuffer::Store(Graphics& gfx, const std::wstring& tag, const DynamicVertex::VertexBuffer& vertexBuffer)
	{
		assert(tag != L"?");
		return Repository::Store<VertexBuffer>(gfx, tag, vertexBuffer);
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
