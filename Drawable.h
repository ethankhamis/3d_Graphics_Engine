#pragma once
#include <DirectXMath.h>
#include "Graphics.h"
#include "debugdefs.h"
#include <memory> // for shared_ptr

namespace Bind
{
	struct Bindable;
	struct IndexBuffer;
}
struct Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable& ) = delete;

public: // standard member functions
	virtual DirectX::XMMATRIX FetchTransformMat() const noexcept = 0; //make abstract
	void Render(Graphics& gfx) const noexcept_unless; // noexcept unless debugging (preprocessor def'd)
	virtual ~Drawable() = default;
protected: // member functions involving adding
	template<class Type>
	Type* QueryBindableObj() noexcept
	{
		for (std::unique_ptr<Bind::Bindable>& pbind : allBinds)
		{
			if (auto pType = dynamic_cast<Type*>(pbind.get()))
			{
				return pType;
			}
		}
		return nullptr;
	}
	void ApplyBind(std::shared_ptr<Bind::Bindable> bind) noexcept_unless;
private:
	const Bind::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bind::Bindable>> allBinds;
};