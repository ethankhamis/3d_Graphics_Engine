#include "IndexBuffer.h"
#include "ThrowMacros.h"
#include "BindableCodex.h"
namespace Bind
{
	IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices): IndexBuffer(gfx, L"?", indices){}
	IndexBuffer::IndexBuffer(Graphics& gfx, std::wstring tag, const std::vector<UINT16>& indices)
		:
		count(static_cast<unsigned int>(indices.size())),
		tag(tag)
	{
		DEF_INFOMANAGER(gfx);
		D3D11_BUFFER_DESC indexbufferDesc = {};

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
	std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(Graphics& gfx, const std::wstring& tag, const std::vector<UINT16> indices)
	{
		assert(tag != L"?");
		return Codex::Resolve<IndexBuffer>(gfx, tag, indices);
	}
	std::wstring IndexBuffer::FetchUID() const noexcept
	{
		return ConstructUID_(tag);
	}
	std::wstring IndexBuffer::ConstructUID_(const std::wstring& tag)
	{
		using namespace std::string_literals;
		return convert::make_wstring( typeid(IndexBuffer).name()) + L"#"s + tag;
	}
}