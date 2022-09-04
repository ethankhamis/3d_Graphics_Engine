#pragma once
#include "DrawableBase.h"

struct BindSolidPlane : public DrawableBase<BindSolidPlane>
{
	BindSolidPlane(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_int_distribution<int>& hDiv,
		std::uniform_int_distribution<int>& vDiv);
	void Update(float deltaTime) noexcept override;
	DirectX::XMMATRIX FetchTransformMat() const noexcept override;

private:
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;

	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};