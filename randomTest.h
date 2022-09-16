#pragma once

#include "ObjTestCase.h"
#include "ConstBuffer(s).h"

struct RandomTest : public TestObject<RandomTest>
{
	RandomTest(
		Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material,
		float scale
	);
};