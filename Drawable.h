#pragma once
#include <DirectXMath.h>
#include "Graphics.h"

struct Bindable;

struct Drawable
{
	Drawable() = default;
	Drawable(const Drawable& ) = delete;

public: // standard member functions
	virtual DirectX::XMMATRIX FetchTransformMat() const noexcept = 0; //make abstract
	void Render(Graphics& gfx) const noexcept (!Debug); // noexcept unless debugging (preprocessor def'd)
	virtual void Update(float DeltaTime) noexcept = 0;
	virtual ~Drawable() = default;
	
public: // member functions involving adding
	void addIndexBuf(std::unique_ptr<struct IndexBuffer> indexBuffer) noexcept;
	void addBind(std::unique_ptr<Bindable> bind) noexcept (!Debug);
protected:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> allBinds;
};