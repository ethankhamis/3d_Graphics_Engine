#pragma once
#include <DirectXMath.h>
#include "ConstBuffer(s).h"
#include "Drawable.h"

struct TransformConstBuffer : public Bindable
{
	TransformConstBuffer(Graphics& gfx, const Drawable& parent) noexcept;
public:
	void Bind(Graphics& gfx) noexcept override;
private:
	static  std::unique_ptr<VConstantBuffer<DirectX::XMMATRIX>> pVConstBuffer;
private:
	const Drawable& parent;
};