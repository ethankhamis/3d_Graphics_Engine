#pragma once
#include <optional>
#include <DirectXMath.h>
#include "Math.cpp"
#include "IndexedTriangleList.h"
#include "DynamicVertex.h"

using std::vector;
struct Sphere
{
	static IndexedTriangleList Create_Advanced(DynamicVertex::VertexLayout vlayout,int Division_Latitude, int Division_Longitude);
	static IndexedTriangleList Create( std::optional<DynamicVertex::VertexLayout> vlayout = std::nullopt)
	{
		if (!vlayout)
		{
			vlayout = DynamicVertex::VertexLayout{}.Append(DynamicVertex::VertexLayout::MemberType::Position3D);
		}
		return Create_Advanced(std::move(*vlayout), 12, 24);
		
	}


};


inline IndexedTriangleList Sphere::Create_Advanced(DynamicVertex::VertexLayout vlayout, int Division_Latitude, int Division_Longitude)
{
	assert(Division_Latitude >= 3);
	assert(Division_Longitude >= 3);

	constexpr float radius = 1.0f;
	const auto base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
	const float lattitudeAngle = static_cast<float>(PI / Division_Latitude);
	const float longitudeAngle = static_cast<float>(2.0f * PI / Division_Longitude);

	DynamicVertex::VertexBuffer vbuffer{ std::move(vlayout) };
	for (int iLat = 1; iLat < Division_Latitude; iLat++)
	{
		const auto latBase = DirectX::XMVector3Transform(
			base,
			DirectX::XMMatrixRotationX(lattitudeAngle * iLat)
		);
		for (int iLong = 0; iLong < Division_Longitude; iLong++)
		{
			DirectX::XMFLOAT3 calculated_position;
			auto v = DirectX::XMVector3Transform(
				latBase,
				DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			DirectX::XMStoreFloat3(&calculated_position, v);
			vbuffer.Emplace_Back(calculated_position);
		}
	}

	// add the cap vertices
	const UINT16 iNorthPole = (unsigned short)vbuffer.size();
{
	DirectX::XMFLOAT3 north_position;
	DirectX::XMStoreFloat3(&north_position, base);
	vbuffer.Emplace_Back(north_position);
}
	const UINT16 iSouthPole = (unsigned short)vbuffer.size();
	{
		DirectX::XMFLOAT3 south_position;
		DirectX::XMStoreFloat3(&south_position, DirectX::XMVectorNegate(base));
		vbuffer.Emplace_Back(south_position);
	}
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

	return { std::move(vbuffer),std::move(indices) };
}
