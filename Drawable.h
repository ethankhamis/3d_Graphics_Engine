#pragma once
#include <DirectXMath.h>
#include "Graphics.h"

struct Bindable;

struct Drawable
{
	template<class T>
	friend struct DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable& ) = delete;

public: // standard member functions
	virtual DirectX::XMMATRIX FetchTransformMat() const noexcept = 0; //make abstract
	void Render(Graphics& gfx) const noexcept (!Debug); // noexcept unless debugging (preprocessor def'd)
	virtual void Update(float DeltaTime) noexcept = 0;
	virtual ~Drawable() = default;
	
protected: // member functions involving adding
	void ApplyIndexBuffer(std::unique_ptr<struct IndexBuffer> indexBuffer) noexcept (!Debug);
	void ApplyBind(std::unique_ptr<Bindable> bind) noexcept (!Debug);
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& FetchStaticBinds() const noexcept = 0;
private:
	const struct IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> allBinds;
};