#pragma once
#include "DrawableBase.h"

struct SolidLongLatSphere : public DrawableBase<SolidLongLatSphere>
{
	SolidLongLatSphere(
		Graphics& gfx, float rad);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX FetchTransformMat() const noexcept override;
	void SetPosition(DirectX::XMFLOAT3 position);

private:
	DirectX::XMFLOAT3 position = { 1.f , 1.f , 1.f };
};