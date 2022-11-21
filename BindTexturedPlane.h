#pragma once
#include "Drawable.h"
#include "Math.cpp"

struct TexturedPlane : public Drawable
{
	TexturedPlane(Graphics& gfx, float size, std::wstring texture);
	void ApplyPos(float3 pos) noexcept;
	void ApplyRot(float roll, float pitch, float yaw) noexcept;
	matrix FetchTransformMat() const noexcept override;
private:
	float3 position = { 1.f,1.f,1.f };
	float roll = 0.f, 
		  pitch = 0.f,
		  yaw = 0.f;
};