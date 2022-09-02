#pragma once
#include "Bindable.h"
#include "ThrowMacros.h"

using Microsoft::WRL::ComPtr;

struct VertexBuffer : public Bindable
{
	template <class Vertex>
	VertexBuffer(Graphics& gfx, const std::vector<Vertex>& vertices) : stride(sizeof(Vertex))
	{
		DEF_INFOMANAGER(gfx);

		D3D11_BUFFER_DESC bufferDesc = {};
		
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
		bufferDesc.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA SubresourceData = {};
		SubresourceData.pSysMem = vertices.data();
		GFX_THROW_INFO(FetchDevice(gfx)->CreateBuffer(&bufferDesc, &SubresourceData, &pVertexBuffer));
	}
	void Bind(Graphics& gfx) noexcept override;
protected:
	UINT stride;
	ComPtr<ID3D11Buffer> pVertexBuffer;
};
