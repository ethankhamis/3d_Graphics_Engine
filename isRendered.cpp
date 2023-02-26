#pragma once
#include "isRendered.h"
#include "ThrowMacros.h"
#include <typeinfo>
#include "IndexBuffer.h"
#include <cassert>

using namespace Bind;

void isRendered::Render(Graphics& gfx) const noexcept_unless
{
	for (auto& bind : allBinds){ bind->Bind(gfx); }
	//render geometry using count from the index buffer 
	gfx.RenderIndexed(pIndexBuffer->FetchCount());
}


void isRendered::ApplyBind(std::shared_ptr<Bind::isBinded> bind) noexcept_unless
{
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert( "index buffer already exists for mesh" && pIndexBuffer == nullptr );
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	allBinds.push_back(std::move(bind));

}
