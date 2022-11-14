
#pragma once


//#include "Math.cpp"

/*testcase taken from ChilliTomatoNoodle*/
/*
template<class Object>
struct TestObject : public DrawableBase<Object>
{
	TestObject
	(
		Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist
	)
		:
		r(rdist(rng)),
		droll(ddist(rng)),
		dpitch(ddist(rng)),
		dyaw(ddist(rng)),
		dphi(odist(rng)),
		dtheta(odist(rng)),
		dchi(odist(rng)),
		chi(adist(rng)),
		theta(adist(rng)),
		phi(adist(rng))
	{
	}

	void Update(float deltaTime) noexcept
	{
		roll   = wrap_Radians( roll + droll * deltaTime);
		pitch  = wrap_Radians( pitch + dpitch * deltaTime);
		yaw    = wrap_Radians( yaw + dyaw * deltaTime);
		theta  = wrap_Radians( theta + dtheta * deltaTime);
		phi    = wrap_Radians( phi + dphi * deltaTime);
		chi    = wrap_Radians( chi + dchi * deltaTime);
	}
	DirectX::XMMATRIX FetchTransformMat() const noexcept
	{
		return
			DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
			*
			DirectX::XMMatrixTranslation(r, .0f, .0f)
			*
			DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}

protected:
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
};
*/