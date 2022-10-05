#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"
#include "debugdefs.h"

template<class T>
struct DrawableBase : public Drawable
{
protected:
	static bool is_static_init() noexcept
	{
		return !staticBinds.empty();
	}
	static void ApplyStaticBind(std::unique_ptr<Bind::Bindable> bind) noexcept_unless
	{
		assert("*Must* use ApplyIndexBufferfer to bind index buffer" && typeid(*bind) != typeid(Bind::IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}
	void ApplyStaticIndexBuffer(std::unique_ptr<Bind::IndexBuffer> ibuf) noexcept_unless
	{
		assert(pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}
	void ApplyIndexFromStatic() noexcept_unless
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<Bind::IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
	}
private:
	const std::vector<std::unique_ptr<Bind::Bindable>>& FetchStaticBinds() const noexcept override
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bind::Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bind::Bindable>> DrawableBase<T>::staticBinds;