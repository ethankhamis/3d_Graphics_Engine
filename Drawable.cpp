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
	for (auto& bind : FetchStaticBinds()) { bind->Bind(gfx); }
	
	gfx.RenderIndexed(pIndexBuffer->FetchCount());
}

void Drawable::ApplyIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) noexcept_unless
{
	assert("Adding Index Buffer (2nd Attempt)" &&
		pIndexBuffer == nullptr);
	pIndexBuffer = indexBuffer.get();
	allBinds.push_back(std::move(indexBuffer));
}

void Drawable::ApplyBind(std::unique_ptr<Bind::Bindable> bind) noexcept_unless
{
	assert("MUST USE FUNC 'ApplyIndexBuffer' when binding index buffer" && typeid(*bind) != typeid(IndexBuffer));
	allBinds.push_back(std::move(bind));

}
