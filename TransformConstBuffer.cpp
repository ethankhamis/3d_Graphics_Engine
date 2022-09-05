#include "TransformConstBuffer.h"

	TransformConstBuffer::TransformConstBuffer(Graphics & gfx, const Drawable & parent, UINT slot) noexcept
		:
		parent(parent)
	{
		if (!pVConstBuffer) {pVConstBuffer = std::make_unique<VConstantBuffer<allTransforms>>(gfx,slot);} //create buffer if not already created
	}

	void TransformConstBuffer::Bind(Graphics& gfx) noexcept
{
		const DirectX::XMMATRIX modelView = parent.FetchTransformMat() * gfx.FetchCameraMat();
	
		const allTransforms transforms =
		{
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose
			(
				modelView *
				gfx.FetchProjection()
			)
		};

	pVConstBuffer->Update(gfx,transforms);
	pVConstBuffer->Bind(gfx);
}

	std::unique_ptr<VConstantBuffer<TransformConstBuffer::allTransforms>> TransformConstBuffer::pVConstBuffer;