#pragma once

#include "TransformConstBuffer.h"
#include <memory>

namespace Bind
{
	struct PrimaryTransformConstBuffer : public TransformConstBuffer
	{
		//conbstructor
		PrimaryTransformConstBuffer(Graphics& gfx, const isRendered& parent,
			unsigned int vertex_slot = NULLUNSIGNED, unsigned int pixel_slot=NULLUNSIGNED);

		//overriden Bind function from isBinded
		void Bind(Graphics& gfx) noexcept override;
		
	protected:
		//own update function for pixel buffer
		void UpdateBind(Graphics& gfx, const allTransforms& transforms) noexcept;
	private:
		static std::unique_ptr<PixelConstantBuffer<allTransforms>> pPixelConstBuffer;
	};
}