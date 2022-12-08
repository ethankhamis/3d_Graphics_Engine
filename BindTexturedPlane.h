#pragma once
#include "Drawable.h"
#include "Math.cpp"
#include <optional>

struct TexturedPlane : public Drawable
{
	TexturedPlane(Graphics& gfx, float size, std::wstring texture, std::optional<std::wstring> normal);
	void ApplyPos(float3 pos) noexcept;
	void ApplyRot(float roll, float pitch, float yaw) noexcept;
	matrix FetchTransformMat() const noexcept override;
public:
	void Open_Window(Graphics& gfx) noexcept;
private:
	float3 position = { 1.f,1.f,1.f };
	float roll = 0.f, 
		  pitch = 0.f,
		  yaw = 0.f;
	struct PixelShaderMaterialProperties
	{
		float
			specular_intensity = .1f,
			specular_power = 20.f;
		int normal_map_state = 1;
		float padding[1];
	}pixelshaderproperties;
};