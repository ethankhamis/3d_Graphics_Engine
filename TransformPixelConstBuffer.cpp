#include "PrimaryTransformConstBuffer.h"

namespace Bind
{
	Bind::PrimaryTransformConstBuffer::PrimaryTransformConstBuffer(Graphics& gfx, const isRendered& parent, UINT32 vertex_slot, UINT32 pixel_slot)
		:TransformConstBuffer(gfx, parent, vertex_slot)
	{
		if (!pPixelConstBuffer)
		{
			pPixelConstBuffer = std::make_unique<PixelConstantBuffer<allTransforms>>(gfx,pixel_slot);
		}
	}

	void Bind::PrimaryTransformConstBuffer::Bind(Graphics& gfx) noexcept
	{
		allTransforms transforms = FetchTransforms(gfx);
		//update bind for vertex buffer
		TransformConstBuffer::UpdateBind(gfx, transforms);
		//update bind for pixel buffer
		UpdateBind(gfx, transforms);
	}

	void Bind::PrimaryTransformConstBuffer::UpdateBind(Graphics& gfx, const allTransforms& transforms) noexcept
	{
		pPixelConstBuffer->Update(gfx, transforms);
		pPixelConstBuffer->Bind(gfx);
	}

	std::unique_ptr<PixelConstantBuffer<TransformConstBuffer::allTransforms>> PrimaryTransformConstBuffer::pPixelConstBuffer;
}