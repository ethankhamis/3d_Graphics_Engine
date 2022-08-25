#include "TransformConstBuffer.h"

TransformConstBuffer::TransformConstBuffer(Graphics& gfx, const Drawable& parent)
:
	vConstBuffer(gfx),
	parent(parent)
{}

void TransformConstBuffer::Bind(Graphics& gfx) noexcept
{
	vConstBuffer.Update
	(gfx, DirectX::XMMatrixTranspose(parent.FetchTransformMat() * gfx.FetchProjection()));

	vConstBuffer.Bind(gfx);
}
