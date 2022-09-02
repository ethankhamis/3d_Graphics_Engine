#include "IndexBuffer.h"
#include "ThrowMacros.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
:
count(static_cast<unsigned int>( indices.size() ))
{
	DEF_INFOMANAGER(gfx);
	D3D11_BUFFER_DESC indexbufferDesc={};

	indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexbufferDesc.CPUAccessFlags = 0u;
	indexbufferDesc.MiscFlags = 0u;
	indexbufferDesc.ByteWidth = UINT(count * sizeof(unsigned short));
	indexbufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indexSubResouceData = {};
	indexSubResouceData.pSysMem = indices.data();
	GFX_THROW_INFO(FetchDevice(gfx)->CreateBuffer(&indexbufferDesc, &indexSubResouceData, &pIndexBuffer));

}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	FetchDeviceContext(gfx)
		->
		IASetIndexBuffer(pIndexBuffer.Get(),
			DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::FetchCount() const noexcept
{
	return count;
}
