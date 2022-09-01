#pragma once
//#include "Drawable.h"
#include <random>
#include "DrawableBase.h"

struct BindCube : public DrawableBase<BindCube>
{
	BindCube(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist
	);

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX FetchTransformMat() const noexcept override;
private:
	// position-oriented
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;

	DirectX::XMFLOAT3X3 model_transform;
};