#pragma once
#include <cmath>
#include <concepts>
#include <DirectXMath.h>

#define PI 3.141592654
#define PI_LONG 3.14159265358979323846 

#define GOLDEN_RATIO 1.61803
#define GOLDEN_RATIO_LONG 1.6180339887 

#define EULERS_NUMBER 2.71828
#define EULERS_NUMBER_LONG 2.71828182845

#define EULERS_CONSTANT 0.57721
#define EULERS_CONSTANT_LONG 0.5772156649


typedef DirectX::XMFLOAT2 float2;
typedef DirectX::XMFLOAT3 float3;
typedef DirectX::XMFLOAT4 float4;
typedef DirectX::XMVECTOR vec32_t;
typedef DirectX::XMMATRIX matrix;
typedef DirectX::XMFLOAT4X4 float4x4;
typedef DirectX::XMVECTOR vector;

template<typename T>
concept Numeric_T = requires (T a)
{
	//create concept which encompasses all numerical types 
	{a} -> std::convertible_to<long long int>;
};

template <Numeric_T N>
N wrap_Radians(N theta)
{
	//initialise pi
	double pi = PI_LONG;
	//wrap the current radians to be within 2pi (a full circle)
	const N modulus = std::fmod(theta, static_cast<N>(2.0f) * static_cast<N>(pi));

	if (modulus > static_cast<N>(pi))
	{ // if the remainder is more than pi, return the modulus multiplied by 2pi
		return modulus - static_cast<N>(2.0f) * static_cast<N>(pi);
	}
	//return the modolus
	return modulus;
}

template <Numeric_T N>
N convert_to_rad(N degrees)
{
	//divide pi by 180 degrees for radian conversion
	float rad_conversion = PI_LONG / static_cast<N>(180);
	//multiply the degrees by the radian conversion
	return degrees * rad_conversion;
}

template <Numeric_T N>
constexpr N interpolate(const N& source, const N& dest, float alpha)
{
	//find the delta of the destination and the source
	float dif = static_cast<float>(dest - source);
	//multiply the difference by the alpha amd add to the source
	return static_cast<float>(source) + (dif * alpha);
}

template <Numeric_T N>
const N Sqrd(N number)
{
	//multiply the number by itself
	return (number * number);
}



