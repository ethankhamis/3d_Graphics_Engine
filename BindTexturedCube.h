#pragma once
#include "isRendered.h"
#include "Math.cpp"
#include <optional>
#include "TypeDecl.h"
struct TexturedCube : public isRendered
{
	TexturedCube(Graphics& gfx, float size, std::wstring texture, std::optional<std::wstring> normal);
	void ApplyPos(float3 pos) noexcept;
	void ApplyRot(float roll, float pitch, float yaw) noexcept;
	matrix FetchTransformMat() const noexcept override;
public:
	void Open_Window(Graphics& gfx) noexcept;
private:
	object_variables::position pos;
	object_variables::orientation orientation{ 0.f, 0.f, 0.f };
	
	struct PixelShaderMaterialProperties
	{
		float
			specular_intensity = .1f,
			specular_power = 20.f;
		int normal_map_state = 1;
		float padding[1];
	}pixelshaderproperties;
};