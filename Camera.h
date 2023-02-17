#pragma once
#include "Graphics.h"
#include "Math.cpp"

struct Camera
{
	Camera() noexcept;
	matrix FetchMatrix() const noexcept; // return matrix 
	void ConstructControlWindow() noexcept; // create control window
	void Resetv() noexcept; // reset camera
	void Rotate_by(float delta_x, float delta_y) noexcept; // rotate camera matrix
	void Translate_by(float3 translation) noexcept; // alter matrix position
private:
	float3 position;
	float pitch, yaw;
	static constexpr float travel_speed = 12.f, rotation_speed = 0.004f;
};