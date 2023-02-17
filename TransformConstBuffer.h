#pragma once
#include <DirectXMath.h>
#include "ConstBuffer(s).h"
#include "isRendered.h"

namespace Bind
{
	struct TransformConstBuffer : public isBinded
	{
	protected:
		struct allTransforms
		{
			DirectX::XMMATRIX modelView;
			DirectX::XMMATRIX modelViewProjection;
		};
	public:
		TransformConstBuffer(Graphics& gfx, const isRendered& parent, UINT slot = 0u) noexcept;
	public:
		void Bind(Graphics& gfx) noexcept override;
	protected:
		void UpdateBind(Graphics& gfx, const allTransforms& transforms) noexcept;
		allTransforms FetchTransforms(Graphics& gfx) noexcept;
	private:
		static  std::unique_ptr<VConstantBuffer<allTransforms>> pVConstBuffer;
	private:
		const isRendered& parent;
	};
}