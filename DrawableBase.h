#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"

template<class T>
struct DrawableBase : public Drawable
{
protected:
	static bool is_static_init() noexcept
	{
		return !staticBinds.empty();
	}
	static void ApplyStaticBind(std::unique_ptr<Bindable> bind) noexcept(!Debug)
	{
		assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}
	void ApplyStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(!Debug)
	{
		assert(pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}
	void ApplyIndexFromStatic() noexcept(!Debug)
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& FetchStaticBinds() const noexcept override
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;