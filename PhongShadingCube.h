#pragma once
#include "ObjTestCase.h"
#include "ConstBuffer(s).h"

struct PhongShadingCube : public TestObject<PhongShadingCube>
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

	DirectX::XMMATRIX FetchTransformMat() const noexcept override;
	bool ControlWnd(int identifier, Graphics& gfx) noexcept;
private:
	void RefreshMaterial(Graphics& gfx) noexcept (!Debug);
private:
	// model transform
	DirectX::XMFLOAT3X3 model_transform;
private:
	struct MaterialConstPS
	{
		DirectX::XMFLOAT3 colour;
		float specular_intensity = .6f;
		float specularPow = 30.f;
		float padding[3];
	}material_constants;


};