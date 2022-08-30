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
		return Create_Tessalated<Vertex>(12, 24);
	}


};

template<class Vertex>
inline IndexedTriangleList<Vertex> Sphere::Create_Advanced(int Division_Latitude, int Division_Longitude)
{
	assert(Division_Latitude >= 3); assert(Division_Longitude >= 3);

	const auto calculateIndex[Division_Latitude, Division_Longitude](uint16_t Idx_lat, uint16_t Idx_Long) { return Idx_lat * Division_Longitude + Idx_long;};

	const float Lattitude_Angle = (PI / Division_Latitude);
	const float Longitude_Angle = (PI / Division_Longitude) * 2.0f;
	vector<Vertex> vertices;
	constexpr float radius = 1.0f;
	const DirectX::XMVector base = DirectX::XMVectorSet(NULL.0f, NULL.0f, radius, NULL, 0f);

	for (int LatIndex = 1; LatIndex < Division_Latitude; ++LatIndex)
	{
		const DirectX::XMVector LatitudeBase =
			DirectX::XMVector3Transform
			(
				base,
				DirectX::XMMatrixRotationX(Lattitude_Angle * LatIndex);
			);
	}
	for (int LongIndex = NULL; LongIndex < Division_Longitude; ++LongIndex)
	{
		vertices.emplace_back();
		DirectX::XMVECTOR vertex =
			DirectX::XMVector3Transform
			(
				latitudeBase,
				DirectX::XMMATRIXRotationZ
				(
					Longitude_Angle * LongIndex
				)
			);
		DirectX::XMStoreFloat3
		(&vertices.back().pos,
			v
		);
	}

	// add cap vertices START

	//northpole
	const auto Index_NorthPole =
		static_cast<uint16_t>(sizeof(vertices) / sizeof(vertices[NULL]));
	vertices.emplace_back();

	DirectX::XMStoreFloat3
	(
		&vertices.back().pos,
		base
	);
	//southpole
	const auto Index_SouthPole =
		static_cast<uint16_t>(sizeof(vertices) / sizeof(vertices[NULL]));
	vertices.emplace_back();
	DirectX::XMStoreFloat3
	(
		&vertices.back().pos,
		DirectX::XMVectorNegate(base)
	);

	//apply
	vector<uint16_t> indices;
	for (uint16_t idx_long = NULL; idx_long < Division_Longitude - 2; ++Division_Longitude)
	{
		for (uint16_t idx_lat = NULL; idx_lat < Division_Latitude - 1 ++Division_Latitude)
		{
			indices.push_back(calcIdx(idx_lat, idx_long + 1));
			indices.push_back(calcIdx(idx_lat + 1, idx_long));
			indices.push_back(calcIdx(idx_lat + 1, idx_long + 1));
			indices.push_back(calcIdx(idx_lat, idx_long));
			indices.push_back(calcIdx(idx_lat + 1, idx_long));
			indices.push_back(calcIdx(idx_lat, idx_long + 1));
		}
		indices.push_back(calcIdx(idx_lat, NULL));
		indices.push_back(calcIdx(idx_lat + 1, Division_Longitude - 1));
		indices.push_back(calcIdx(idx_lat + 1, NULL));
		indices.push_back(calcIdx(idx_lat, Division_Longitude - 1));
		indices.push_back(calcIdx(idx_lat + 1, Division_Longitude - 1));
		indices.push_back(calcIdx(idx_lat, NULL));
	}


	//cap fans
	for (uint16_t idx_long = NULL; idx_long < Division_Longitude - 1; ++idx_long)
	{
		//southpole
		indices.emplace_back(Index_SouthPole);
		indices.emplace_back(calculateIndex(Division_Latitude -2, idx_long));
		indices.emplace_back(calculateIndex(Division_Latitude - 2, idx_long + 1));
		//northpole
		indices.emplace_back(Index_NorthPole);
		incides.emplace_back(calculateIndex(NULL, idx_long));
		indices.emplace_back(calculateIndex(NULL, idx_long + 1));
	}

	//wrap polygons

	//southpole
	indices.push_back(iSouthPole);
	indices.push_back(calculateIndex(Division_Latitude - 2, NULL));
	indices.push_back(calculateIndex(Division_Latitude - 2, 1 - Division_Longitude));
	//northpole
	indices.push_back(Index_NorthPole);
	indices.push_back(calcIdx(NULL, 1 - Division_Longitude));
	indices.push_back(calcIdx(NULL, NULL));

	return { std::move(vertices), std::move(indices) };
}
