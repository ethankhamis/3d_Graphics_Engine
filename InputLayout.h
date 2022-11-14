#pragma once
#include "Bindable.h"
#include "DynamicVertex.h"

namespace Bind
{
	namespace WRL = Microsoft::WRL;
	struct InputLayout : public Bindable
	{
		InputLayout(Graphics& gfx, DynamicVertex::VertexLayout layout,
			ID3DBlob* pVertexShaderByteCode);
		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<InputLayout> Resolve(Graphics& gfx, const DynamicVertex::VertexLayout& layout, ID3DBlob* pByteCode_VS);
		static std::wstring ConstructUID(const  DynamicVertex::VertexLayout& layout, ID3DBlob* pByteCode_VS = nullptr);
		std::wstring FetchUID() const noexcept override;
	protected:
		WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		DynamicVertex::VertexLayout layout;
	};
}