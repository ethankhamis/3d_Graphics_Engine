#pragma once
#include "ObjTestCase.h"

struct SkinnedCube : public TestObject<SkinnedCube>
{
	SkinnedCube
	(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist
		);

};