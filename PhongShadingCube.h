#pragma once
#include "DrawableBase.h"

struct PhongShadingCube : public DrawableBase<PhongShadingCube>
{
public:
	PhongShadingCube(
		Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_real_distribution<float>& bdist,
		DirectX::XMFLOAT3 material
	);

	void Update(float delta) noexcept override;
	DirectX::XMMATRIX FetchTransformMat() const noexcept override;
private:

	// positional
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
	// model transform
	DirectX::XMFLOAT3X3 model_transform;

};