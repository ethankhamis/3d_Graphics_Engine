#pragma once
#include "Drawable.h"
#include "ThrowMacros.h"
#include <typeinfo>
#include "IndexBuffer.h"
#include <cassert>

using namespace Bind;

void Drawable::Render(Graphics& gfx) const noexcept_unless
{
	for (auto& bind : allBinds){ bind->Bind(gfx); }
	
	gfx.RenderIndexed(pIndexBuffer->FetchCount());
}


void Drawable::ApplyBind(std::shared_ptr<Bind::Bindable> bind) noexcept_unless
{
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert( "MUST USE FUNC 'ApplyIndexBuffer' when binding index buffer" && pIndexBuffer == nullptr );
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	allBinds.push_back(std::move(bind));

}
