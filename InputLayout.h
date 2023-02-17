#pragma once
#include "isBinded.h"
#include "DynamicVertex.h"

namespace Bind
{
	namespace WRL = Microsoft::WRL;
	struct InputLayout : public isBinded
	{
		InputLayout(Graphics& gfx, DynamicVertex::VertexLayout layout,
			ID3DBlob* pVertexShaderByteCode);
		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<InputLayout> Store(Graphics& gfx, const DynamicVertex::VertexLayout& layout, ID3DBlob* pByteCode_VS);
		static std::wstring ConstructUID(const  DynamicVertex::VertexLayout& layout, ID3DBlob* pByteCode_VS = nullptr);
		std::wstring FetchUID() const noexcept override;
	protected:
		WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		DynamicVertex::VertexLayout layout;
	};
}