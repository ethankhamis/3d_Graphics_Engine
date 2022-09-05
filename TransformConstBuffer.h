#pragma once
#include <DirectXMath.h>
#include "ConstBuffer(s).h"
#include "Drawable.h"

struct TransformConstBuffer : public Bindable
{
private:
	struct allTransforms
	{
		DirectX::XMMATRIX modelViewProject;
		DirectX::XMMATRIX model;
	};
public:
	TransformConstBuffer(Graphics& gfx, const Drawable& parent, UINT slot = 0u) noexcept;
public:
	void Bind(Graphics& gfx) noexcept override;
private:
	static  std::unique_ptr<VConstantBuffer<allTransforms>> pVConstBuffer;
private:
	const Drawable& parent;
};