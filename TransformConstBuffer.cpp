#include "TransformConstBuffer.h"

	TransformConstBuffer::TransformConstBuffer(Graphics & gfx, const Drawable & parent) noexcept
		:
		parent(parent)
	{
		if (!pVConstBuffer) {pVConstBuffer = std::make_unique<VConstantBuffer<allTransforms>>(gfx);} //create buffer if not already created
	}

	void TransformConstBuffer::Bind(Graphics& gfx) noexcept
{
		const DirectX::XMMATRIX model = parent.FetchTransformMat();
	
		const allTransforms transforms =
		{
			DirectX::XMMatrixTranspose(model),
			DirectX::XMMatrixTranspose
			(
				model *
				gfx.FetchCameraMat() *
				gfx.FetchProjection()
			)
		};
		pVConstBuffer->Update(gfx,transforms);

	pVConstBuffer->Bind(gfx);
}

	std::unique_ptr<VConstantBuffer<TransformConstBuffer::allTransforms>> TransformConstBuffer::pVConstBuffer;