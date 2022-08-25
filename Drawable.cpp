#pragma once
#include "Drawable.h"
#include "ThrowMacros.h"
#include <typeinfo>
#include "IndexBuffer.h"
#include <cassert>


void Drawable::Render(Graphics& gfx) const noexcept(!Debug)
{
	for (auto& bind : allBinds){ bind->Bind(gfx); }
	for (auto& bind : FetchStaticBinds()) { bind->Bind(gfx); }
	
	gfx.DrawIndexed(pIndexBuffer->FetchCount());
}

void Drawable::addIndexBuf(std::unique_ptr<struct IndexBuffer> indexBuffer) noexcept (!Debug)
{
	assert("Adding Index Buffer (2nd Attempt)" &&
		pIndexBuffer == nullptr);
	pIndexBuffer = indexBuffer.get();
	allBinds.push_back(std::move(indexBuffer));
}

void Drawable::addBind(std::unique_ptr<Bindable> bind) noexcept(!Debug)
{
	assert("MUST USE FUNC 'AddIndexBuf' when binding index buffer" && typeid(*bind) != typeid(IndexBuffer));
	allBinds.push_back(std::move(bind));

}
