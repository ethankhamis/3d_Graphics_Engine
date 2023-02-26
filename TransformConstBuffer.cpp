#include "TransformConstBuffer.h"


namespace Bind
{


	TransformConstBuffer::TransformConstBuffer(Graphics& gfx, const isRendered& parent, UINT slot) noexcept
		:
		parent(parent)
	{
		if (!pVertexConstBuffer) { pVertexConstBuffer = std::make_unique<VConstantBuffer<allTransforms>>(gfx, slot); } //create buffer if not already created
	}

	void TransformConstBuffer::Bind(Graphics& gfx) noexcept
	{
		//update and bind the constant buffer
		UpdateBind(gfx, FetchTransforms(gfx));
	}

	void TransformConstBuffer::UpdateBind(Graphics& gfx, const allTransforms& transforms) noexcept
	{
		//update constant buffer with most recent information
		pVertexConstBuffer->Update(gfx, transforms);
		//bind updated buffer
		pVertexConstBuffer->Bind(gfx);
	}

	TransformConstBuffer::allTransforms TransformConstBuffer::FetchTransforms(Graphics& gfx) noexcept
	{
		//intiialise model view to the transformation matrix multiplied by the camera matrix
		const DirectX::XMMATRIX modelView = parent.FetchTransformMat() * gfx.FetchCameraMat();
		// return the transposed matrices
		// with their row and column flipped to be represented in the correct form for directx
		//directx uses row major matrices which differ from column major's
		//model view multiplied by projection IS the model view projection
		return
		{
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose
			(
				modelView *
				gfx.FetchProjection()
				// fetch projection is the correct camera projection (aapect ratio set in application constructor)
			)
		};
	}

	std::unique_ptr<VConstantBuffer<TransformConstBuffer::allTransforms>> TransformConstBuffer::pVertexConstBuffer;
}