#pragma once
#include "isRendered.h"

struct SolidLongLatSphere : public isRendered
{
	SolidLongLatSphere(
		Graphics& gfx, float rad);
	DirectX::XMMATRIX FetchTransformMat() const noexcept override;
	void SetPosition(DirectX::XMFLOAT3 position);

private:
	DirectX::XMFLOAT3 position = { 1.f , 1.f , 1.f };
};