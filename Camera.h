#pragma once
#include "Graphics.h"

struct Camera
{
	DirectX::XMMATRIX FetchMatrix() const noexcept;
	void ConstructControlWindow() noexcept;
	void Resetv() noexcept;
private:
	float radius = 20.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};