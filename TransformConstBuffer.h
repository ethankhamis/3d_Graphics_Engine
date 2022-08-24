#pragma once
#include <DirectXMath.h>
#include "ConstBuffer(s).h"
#include "Drawable.h"

struct TransformConstBuffer : public Bindable
{
	TransformConstBuffer(Graphics& gfx, const Drawable& parent);
public:
	void Bind(Graphics& gfx) noexcept override;
private:
	VConstantBuffer<DirectX::XMMATRIX> vConstBuffer;
private:
	const Drawable& parent;
};