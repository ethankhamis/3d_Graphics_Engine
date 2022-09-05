#pragma once

#include "Bindable.h"
#include "ThrowMacros.h"

template<typename Ctype>
struct ConstantBuffer : public Bindable
{
	ConstantBuffer(Graphics& gfx, const Ctype& constants, UINT slot = 0u);
	ConstantBuffer(Graphics& gfx, UINT slot = 0u);
	void Update(Graphics& gfx, const Ctype& constants);
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
	UINT slot;
};

template<typename Ctype>
struct PixelConstantBuffer : public ConstantBuffer<Ctype> // Pixel Constant Buffer
{
private:
	using ConstantBuffer<Ctype>::pConstBuffer;
	using ConstantBuffer<Ctype>::slot;
	using Bindable::FetchDeviceContext;
public:
	using ConstantBuffer<Ctype>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override;
};

template <typename Ctype>
struct VConstantBuffer : public ConstantBuffer<Ctype>// Vertex Constant Buffer
{
private:
	using ConstantBuffer<Ctype>::pConstBuffer;
	using ConstantBuffer<Ctype>::slot;
	using Bindable::FetchDeviceContext;
public:
	using ConstantBuffer<Ctype>::ConstantBuffer;
public:
	void Bind(Graphics& gfx) noexcept override;
};

template<typename Ctype>
inline ConstantBuffer<Ctype>::ConstantBuffer(Graphics& gfx, const Ctype& constants, UINT slot)
	:
	slot(slot)
{
	DEF_INFOMANAGER(gfx);

	size_t size = sizeof(constants);

	D3D11_BUFFER_DESC ConstBufDesc;
	ConstBufDesc.MiscFlags = 0u;
	ConstBufDesc.StructureByteStride = 0u;
	ConstBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstBufDesc.ByteWidth = sizeof(constants);
	ConstBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	ConstBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA ConstBufSubResourceData = {};
	ConstBufSubResourceData.pSysMem = &constants;
	GFX_THROW_INFO(FetchDevice(gfx)
		->
		CreateBuffer(&ConstBufDesc, &ConstBufSubResourceData, &pConstBuffer)
	);
}

template<typename Ctype>
inline ConstantBuffer<Ctype>::ConstantBuffer(Graphics& gfx, UINT slot)
:
	slot(slot)
{
	DEF_INFOMANAGER(gfx);

	size_t size = sizeof(Ctype);

	D3D11_BUFFER_DESC ConstBufDesc;
	ConstBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	ConstBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstBufDesc.MiscFlags = 0u;
	ConstBufDesc.ByteWidth = sizeof(Ctype);
	ConstBufDesc.StructureByteStride = 0u;
	GFX_THROW_INFO(FetchDevice(gfx)
		->
		CreateBuffer(&ConstBufDesc, nullptr, &pConstBuffer)
	);
}

template<typename Ctype>
inline void ConstantBuffer<Ctype>::Update(Graphics& gfx, const Ctype& constants)
{
	DEF_INFOMANAGER(gfx);

	D3D11_MAPPED_SUBRESOURCE MappedSubRes;
	GFX_THROW_INFO(FetchDeviceContext(gfx)->Map(
		pConstBuffer.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&MappedSubRes));

	size_t size = sizeof(constants);
	memcpy(MappedSubRes.pData, &constants, sizeof(constants));
	FetchDeviceContext(gfx)->Unmap(pConstBuffer.Get(), 0u);
}

template<typename Ctype>
inline void PixelConstantBuffer<Ctype>::Bind(Graphics& gfx) noexcept
{
	FetchDeviceContext(gfx)
		->
		PSSetConstantBuffers(slot, 1u, pConstBuffer.GetAddressOf());
}

template<typename Ctype>
inline void VConstantBuffer<Ctype>::Bind(Graphics& gfx) noexcept
{
	FetchDeviceContext(gfx)
		->
		VSSetConstantBuffers(slot, 1u, pConstBuffer.GetAddressOf());
}
