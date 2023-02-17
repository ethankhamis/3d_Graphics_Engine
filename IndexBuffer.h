#pragma once

#include "isBinded.h"

namespace Bind
{
	namespace WRL = Microsoft::WRL;
	struct IndexBuffer : public isBinded
	{
		IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
		IndexBuffer(Graphics& gfx, std::wstring tag, const std::vector<UINT16>& indices);
	public:
		void Bind(Graphics& gfx) noexcept override;
	public:
		unsigned int FetchCount() const noexcept;
	public:
		static std::shared_ptr<IndexBuffer> Store(Graphics& gfx, const std::wstring& tag, const std::vector<UINT16> indices);
		template<typename ...Remainder>
		static std::wstring ConstructUID(const std::wstring& tag, Remainder&& ...remainder)
		{
			return ConstructUID_(tag);
		}
		std::wstring FetchUID() const noexcept override;
	private:
		static std::wstring ConstructUID_(const std::wstring& tag);
	protected:
		unsigned int count;
		std::wstring tag;
		WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}