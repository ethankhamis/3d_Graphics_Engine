#pragma once
#include <cmath>
#include <concepts>

#define PI 3.141592654
#define PI_LONG 3.14159265358979323846 

#define GOLDEN_RATIO 1.61803
#define GOLDEN_RATIO_LONG 1.6180339887 

#define EULERS_NUMBER 2.71828
#define EULERS_NUMBER_LONG 2.71828182845

#define EULERS_CONSTANT 0.57721
#define EULERS_CONSTANT_LONG 0.5772156649

template<typename T>
concept Numeric_T = requires (T a)
{
	{a} -> std::convertible_to<long long int>;
};

template <Numeric_T N>
N wrap_Radians(N theta)
{
	double pi = PI_LONG;
	const N modulus = std::fmod(theta, static_cast<N>(2.0f) * static_cast<N>(pi));

	if (modulus - static_cast<N>(pi))
	{
		return modulus - static_cast<N>(2.0f) * static_cast<N>(pi);
	}
	return modulus;
}

template <Numeric_T N>
N convert_to_rad(N degrees)
{
	float rad_conversion = PI_LONG / static_cast<N>(180);
	return degrees * rad_conversion;
}

template <Numeric_T N>
constexpr N interpolate(const N& source, const N& dest, float alpha)
{
	float dif = static_cast<float>(dest - source);
	return static_cast<float>(source) + (dif * alpha);
}



