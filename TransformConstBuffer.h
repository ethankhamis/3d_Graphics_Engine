#pragma once
#include <DirectXMath.h>
#include "ConstBuffer(s).h"
#include "isRendered.h"
#include "Math.cpp"

namespace Bind
{
	struct TransformConstBuffer : public isBinded
	{
	protected: // in order to be available to use by the child classes
		struct allTransforms
		{
			matrix modelView;
			matrix modelViewProjection;
		};
	public:
		TransformConstBuffer(Graphics& gfx, const isRendered& parent, UINT slot = 0u) noexcept;
	public:
		void Bind(Graphics& gfx) noexcept override;
	protected:
		//updates a binds the vConstBuffer
		void UpdateBind(Graphics& gfx, const allTransforms& transforms) noexcept;
		//getter for the allTransforms type
		allTransforms FetchTransforms(Graphics& gfx) noexcept;
	private:
		//stores a binded type as a means of connecting the vertex buffer (constant)
		//and the rendered object (i.e., plane, cube, etc.)
		static  std::unique_ptr<VConstantBuffer<allTransforms>> pVertexConstBuffer;
	private:
		const isRendered& parent;
	};
}