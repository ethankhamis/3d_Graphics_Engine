#pragma once

#include "Bindable.h"

namespace Bind
{
	namespace WRL = Microsoft::WRL;
	struct IndexBuffer : public Bindable
	{
		IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	public:
		void Bind(Graphics& gfx) noexcept override;
	public:
		unsigned int FetchCount() const noexcept;
	protected:
		unsigned int count;
		WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}