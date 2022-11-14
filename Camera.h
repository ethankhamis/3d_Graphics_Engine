#pragma once
#include "Graphics.h"
#include "Math.cpp"

struct Camera
{
	Camera() noexcept;
	matrix FetchMatrix() const noexcept;
	void ConstructControlWindow() noexcept;
	void Resetv() noexcept;
	void Rotate_by(float delta_x, float delta_y) noexcept;
	void Translate_by(float3 translation) noexcept;
private:
	float3 position;
	float pitch, yaw;
	static constexpr float travel_speed = 12.f, rotation_speed = 0.004f;
};