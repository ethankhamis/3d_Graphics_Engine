#pragma once

#include "isBinded.h"
#include "isBinded_Storage.h"
#include "ThrowMacros.h"

namespace Bind
{
	template<typename Ctype>
	struct ConstantBuffer : public isBinded
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
		using isBinded::FetchDeviceContext;
	public:
		using ConstantBuffer<Ctype>::ConstantBuffer;
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<PixelConstantBuffer> Store(Graphics& gfx, const Ctype constants, UINT32 slot = NULL)
		{
			return Repository::Store<PixelConstantBuffer>(gfx, constants, slot);
		}
		static std::shared_ptr<PixelConstantBuffer> Store(Graphics& gfx, UINT32 slot = NULL)
		{
			return Repository::Store<PixelConstantBuffer>(gfx, slot);
		}
		static std::wstring ConstructUID(const Ctype&, UINT32 slot = NULL);
		static std::wstring ConstructUID(UINT32 slot = NULL);
		std::wstring FetchUID() const noexcept override;
	};

	template <typename Ctype>
	struct VConstantBuffer : public ConstantBuffer<Ctype>// Vertex Constant Buffer
	{
	private:
		using ConstantBuffer<Ctype>::pConstBuffer;
		using ConstantBuffer<Ctype>::slot;
		using isBinded::FetchDeviceContext;
	public:
		using ConstantBuffer<Ctype>::ConstantBuffer;
	public:
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<VConstantBuffer> Store(Graphics& gfx, const Ctype constants, UINT32 slot = NULL)
		{
			return Repository::Store<VConstantBuffer>(gfx, constants, slot);
		}
		static std::shared_ptr<VConstantBuffer> Store(Graphics& gfx, UINT32 slot = NULL)
		{
			return Repository::Store<VConstantBuffer>(gfx, slot);
		}
		static std::wstring ConstructUID(const Ctype&, UINT32 slot = NULL);
		static std::wstring ConstructUID(UINT32 slot = NULL);
		std::wstring FetchUID() const noexcept override;
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
		//performs what is essentially a constant cast equivalent from D3D
		DEF_INFOMANAGER(gfx);
		// initialise mapped subresource
		D3D11_MAPPED_SUBRESOURCE MappedSubRes;
		GFX_THROW_INFO(FetchDeviceContext(gfx)->Map(
			pConstBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&MappedSubRes));
		//remap constant buffer
		size_t size = sizeof(constants);
		//copy the memory from the constants to the subresource
		memcpy(MappedSubRes.pData, &constants, size);
		//unmap constant buffer
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
	inline std::wstring PixelConstantBuffer<Ctype>::ConstructUID(const Ctype&, UINT32 slot)
	{
		return ConstructUID(slot);
	}

	template<typename Ctype>
	inline std::wstring PixelConstantBuffer<Ctype>::ConstructUID(UINT32 slot)
	{
		using namespace std::string_literals;
		return convert::make_wstring(typeid(PixelConstantBuffer).name()) + L"#"s + convert::make_wstring(std::to_string(slot).c_str());
	}

	template<typename Ctype>
	inline std::wstring PixelConstantBuffer<Ctype>::FetchUID() const noexcept
	{
		return ConstructUID(slot);
	}

	template<typename Ctype>
	inline void VConstantBuffer<Ctype>::Bind(Graphics& gfx) noexcept
	{
		FetchDeviceContext(gfx)
			->
			VSSetConstantBuffers(slot, 1u, pConstBuffer.GetAddressOf());
	}




	template<typename Ctype>
	inline std::wstring VConstantBuffer<Ctype>::ConstructUID(const Ctype&, UINT32 slot)
	{
		return ConstructUID(slot);
	}

	template<typename Ctype>
	inline std::wstring VConstantBuffer<Ctype>::ConstructUID(UINT32 slot)
	{
		using namespace std::string_literals;
		return convert::make_wstring(typeid(VConstantBuffer).name()) + L"#"s + convert::make_wstring(std::to_string(slot).c_str());
	}

	template<typename Ctype>
	inline std::wstring VConstantBuffer<Ctype>::FetchUID() const noexcept
	{
		return ConstructUID(slot);
	}

}