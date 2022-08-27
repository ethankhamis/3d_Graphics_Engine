#include "TransformConstBuffer.h"

	TransformConstBuffer::TransformConstBuffer(Graphics & gfx, const Drawable & parent) noexcept
		:
		parent(parent)
	{
		if (!pVConstBuffer) {pVConstBuffer = std::make_unique<VConstantBuffer<DirectX::XMMATRIX>>(gfx);} //create buffer if not already created
	}

	void TransformConstBuffer::Bind(Graphics& gfx) noexcept
{
	pVConstBuffer->Update
	(gfx, DirectX::XMMatrixTranspose(parent.FetchTransformMat() * gfx.FetchProjection()));

	pVConstBuffer->Bind(gfx);
}

std::unique_ptr<VConstantBuffer<DirectX::XMMATRIX>> TransformConstBuffer::pVConstBuffer;