#pragma once
#include <DirectXMath.h>
#include "MathematicalConstants.h"
#include "IndexedTriangleList.h"

using std::vector;
struct Sphere
{
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_Advanced(int Division_Latitude, int Division_Longitude);
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create()
	{
		return Create_Advanced<Vertex>(12, 24);
	}


};

template<class Vertex>
inline IndexedTriangleList<Vertex> Sphere::Create_Advanced(int Division_Latitude, int Division_Longitude)
{
	namespace dx = DirectX;
	assert(Division_Latitude >= 3);
	assert(Division_Longitude >= 3);

	constexpr float radius = 1.0f;
	const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
	const float lattitudeAngle = PI / Division_Latitude;
	const float longitudeAngle = 2.0f * PI / Division_Longitude;

	std::vector<Vertex> vertices;
	for (int iLat = 1; iLat < Division_Latitude; iLat++)
	{
		const auto latBase = dx::XMVector3Transform(
			base,
			dx::XMMatrixRotationX(lattitudeAngle * iLat)
		);
		for (int iLong = 0; iLong < Division_Longitude; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				latBase,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
		}
	}

	// add the cap vertices
	const auto iNorthPole = (unsigned short)vertices.size();
	vertices.emplace_back();
	dx::XMStoreFloat3(&vertices.back().pos, base);
	const auto iSouthPole = (unsigned short)vertices.size();
	vertices.emplace_back();
	dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

	const auto calcIdx = [Division_Latitude, Division_Longitude](unsigned short iLat, unsigned short iLong)
	{ return iLat * Division_Longitude + iLong; };
	std::vector<unsigned short> indices;
	for (unsigned short iLat = 0; iLat < Division_Latitude - 2; iLat++)
	{
		for (unsigned short iLong = 0; iLong < Division_Longitude - 1; iLong++)
		{
			indices.push_back(calcIdx(iLat, iLong));
			indices.push_back(calcIdx(iLat + 1, iLong));
			indices.push_back(calcIdx(iLat, iLong + 1));
			indices.push_back(calcIdx(iLat, iLong + 1));
			indices.push_back(calcIdx(iLat + 1, iLong));
			indices.push_back(calcIdx(iLat + 1, iLong + 1));
		}
		// wrap band
		indices.push_back(calcIdx(iLat, Division_Longitude - 1));
		indices.push_back(calcIdx(iLat + 1, Division_Longitude - 1));
		indices.push_back(calcIdx(iLat, 0));
		indices.push_back(calcIdx(iLat, 0));
		indices.push_back(calcIdx(iLat + 1, Division_Longitude - 1));
		indices.push_back(calcIdx(iLat + 1, 0));
	}

	// cap fans
	for (unsigned short iLong = 0; iLong < Division_Longitude - 1; iLong++)
	{
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, iLong));
		indices.push_back(calcIdx(0, iLong + 1));
		// south
		indices.push_back(calcIdx(Division_Latitude - 2, iLong + 1));
		indices.push_back(calcIdx(Division_Latitude - 2, iLong));
		indices.push_back(iSouthPole);
	}
	// wrap triangles
	// north
	indices.push_back(iNorthPole);
	indices.push_back(calcIdx(0, Division_Longitude - 1));
	indices.push_back(calcIdx(0, 0));
	// south
	indices.push_back(calcIdx(Division_Latitude - 2, 0));
	indices.push_back(calcIdx(Division_Latitude - 2, Division_Longitude - 1));
	indices.push_back(iSouthPole);

	return { std::move(vertices),std::move(indices) };
}
