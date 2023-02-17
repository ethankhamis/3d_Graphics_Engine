#pragma once
#include <DirectXMath.h>
#include "Graphics.h"
#include "debugdefs.h"
#include <memory> // for shared_ptr

namespace Bind
{
	struct isBinded;
	struct IndexBuffer;
}
struct isRendered
{
public:
	isRendered() = default;
	isRendered(const isRendered& ) = delete;

public: // standard member functions
	virtual DirectX::XMMATRIX FetchTransformMat() const noexcept = 0; //make abstract
	void Render(Graphics& gfx) const noexcept_unless; // noexcept unless debugging (preprocessor def'd)
	virtual ~isRendered() = default;
protected: // member functions involving adding
	template<class Type>
	Type* QueryBindableObj() noexcept
	{
		for (std::shared_ptr<Bind::isBinded>& pbind : allBinds)
		{
			if (auto pType = dynamic_cast<Type*>(pbind.get()))
			{
				return pType;
			}
		}
		return nullptr;
	}
	void ApplyBind(std::shared_ptr<Bind::isBinded> bind) noexcept_unless;
private:
	const Bind::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bind::isBinded>> allBinds;
};