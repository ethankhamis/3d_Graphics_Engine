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
			vlayout = DynamicVertex::VertexLayout{}.Emplace_Back(DynamicVertex::VertexLayout::MemberType::Position3D);
		}
		return Create_Advanced(std::move(*vlayout), 12, 24);
		
	}


};


inline IndexedTriangleList Sphere::Create_Advanced(DynamicVertex::VertexLayout vlayout, int Division_Latitude, int Division_Longitude)
{
	assert(Division_Latitude >= 3);
	assert(Division_Longitude >= 3);

	constexpr float radius = 1.0f; // define radius
	// intialise base where z = 1.f
	const auto base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
	//calculate both longituninal and latitudinal angles for wrapping
	const float lattitudeAngle = static_cast<float>(PI / Division_Latitude);
	const float longitudeAngle = static_cast<float>(2.0f * PI / Division_Longitude);

	//forward the original layout to a new object of VertexLayout
	DynamicVertex::VertexBuffer vbuffer{ std::move(vlayout) };
	for (int iLat = 1; iLat < Division_Latitude; iLat++)
	{
		// construct base by multiplying lattitude by the index and transforming it with the original base
		const auto latBase = DirectX::XMVector3Transform(
			base,
			DirectX::XMMatrixRotationX(lattitudeAngle * iLat)
		);
		for (int iLong = 0; iLong < Division_Longitude; iLong++)
		{
			//calculate vertex position by transforming the latitudinal base by the angle and multiplying by its index
			DirectX::XMFLOAT3 calculated_position;
			auto v = DirectX::XMVector3Transform(
				latBase,
				DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			//store vertex position and emplace it
			DirectX::XMStoreFloat3(&calculated_position, v);
			vbuffer.Emplace_Back(calculated_position);
		}
	}

	// add the cap vertices together
	// use vertices to calculate the tootal vertices 
	const UINT16 iNorthPole = static_cast<unsigned short>(vbuffer.size());
	//store base rvalue within north position
	float3 north_position;
	DirectX::XMStoreFloat3(&north_position, base);
	vbuffer.Emplace_Back(north_position);

	const UINT16 iSouthPole = static_cast<unsigned short>(vbuffer.size());
	{
		float3 south_position;
		//store south position as the opposite of base and emplace it to the vertex buffer
		DirectX::XMStoreFloat3(&south_position, DirectX::XMVectorNegate(base));
		vbuffer.Emplace_Back(south_position);
	}
	//lambda function with the purpose of calculating the indices when given Ilat and Ilong
	const auto calcIdx = [Division_Latitude, Division_Longitude](unsigned short iLat, unsigned short iLong)
	{ return iLat * Division_Longitude + iLong; };
	std::vector<unsigned short> indices;
	for (unsigned short iLat = 0; iLat < Division_Latitude - 2; iLat++)
	{
		for (unsigned short iLong = 0; iLong < Division_Longitude - 1; iLong++)
		{
			//emplace two polygons worth of divisions
			indices.push_back(calcIdx(iLat, iLong));
			indices.push_back(calcIdx(iLat + 1, iLong));
			indices.push_back(calcIdx(iLat, iLong + 1));
			indices.push_back(calcIdx(iLat, iLong + 1));
			indices.push_back(calcIdx(iLat + 1, iLong));
			indices.push_back(calcIdx(iLat + 1, iLong + 1));
		}
		// emplace the wrapping band
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
		// north pole
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, iLong));
		indices.push_back(calcIdx(0, iLong + 1));
		// south pole
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
	// forward the vertex layout and indices in the return statement
	return { std::move(vbuffer),std::move(indices) };
}
