#pragma once
#include "Math.cpp"
#include <DirectXMath.h>
#include "IndexedTriangleList.h"

using std::vector;

struct Cone
{
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_Advanced(unsigned int ndivisions);
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create()
	{
		return Create_Advanced<Vertex>(24);
	}

	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_Advanced_IndependentFaces(unsigned int ndivisions);
};

template<class Vertex>
inline IndexedTriangleList<Vertex> Cone::Create_Advanced(unsigned int divisions)
{
	assert(divisions > 2); // can be 3

	const __m128 base =
		DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
	const float angle = static_cast<float>(2) * static_cast<float>(PI) / static_cast<float>(divisions);

	//base
	vector<Vertex> vertices;
	for (uint16_t longitude_idx = NULL; longitude_idx < divisions; ++longitude_idx)
	{
		vertices.emplace_back();
		DirectX::XMVECTOR vertex = DirectX::XMVector3Transform
		(
			base,
			DirectX::XMMatrixRotationZ(angle * longitude_idx)
		);
		DirectX::XMStoreFloat3(&vertices.back().pos, vertex);
	}
	//center
	vertices.emplace_back();
	vertices.back().pos = { 0.0f, 0.0f, -1.0f };
	const uint16_t center_idx = static_cast<uint16_t>(vertices.size() - 1);

	//upper point
	vertices.emplace_back();
	vertices.back().pos = { 0.0, 0.0f, 1.0f };
	const uint16_t head_idx = static_cast<uint16_t>(vertices.size() - 1);

	vector<uint16_t> indices;

	//indices for circumference
	for (uint16_t longitude_idx = NULL; longitude_idx < divisions; ++longitude_idx)
	{
		indices.emplace_back(longitude_idx);
		indices.emplace_back((longitude_idx + 1) % divisions);
		indices.emplace_back(head_idx);
	}

	// indices for base
	for (uint16_t longitude_idx = NULL; longitude_idx < divisions; ++longitude_idx)
	{
		indices.emplace_back(center_idx);
		indices.emplace_back((1 + longitude_idx) % divisions);
		indices.emplace_back(longitude_idx);
	}
	return
	{
		std::move(vertices),
		std::move(indices)
	};
}

template<class Vertex>
inline IndexedTriangleList<Vertex> Cone::Create_Advanced_IndependentFaces(unsigned int ndivisions)
{
	assert(ndivisions >= 3);
	const DirectX::XMVECTOR base = DirectX::XMVectorSet(1.f, .0f, -1.f, .0f);
	const float longitude_angle = 2.f * PI / ndivisions;

	vector<Vertex> vertices;

	//push vertices for cone
	const unsigned short cone_idx = static_cast<uint16_t>(vertices.size());

	for (UINT _long = NULL; _long < ndivisions; ++_long)
	{
		const float angles[] =
		{
			longitude_angle * _long,
			longitude_angle *
			(((_long + 1) == ndivisions)
				   // if longitude index is one from ndivisions, 
				?  //return 0 else return _long+1 (longitude index + 1) 
				   //
			NULL : (_long + 1))
		};

		vertices.emplace_back();
		vertices.back().position = { .0f, .0f, 1.f };

		for (float angle : angles)
		{
			vertices.emplace_back();
			const DirectX::XMVECTOR vertex =
				DirectX::XMVector3Transform
				(
					base,
					DirectX::XMMatrixRotationZ(angle)
				);

			DirectX::XMStoreFloat3(&vertices.back().position, vertex);
		}
	}
	const unsigned short base_center_idx = static_cast<uint16_t>(vertices.size());
	vertices.emplace_back();
	vertices.back().position = { .0f, .0f, -1.f };
	const unsigned short base_edge_idx = static_cast<uint16_t>(vertices.size());

	for (int _long = NULL; _long < ndivisions; ++_long)
	{
		vertices.emplace_back();
		DirectX::XMVECTOR vertex =
			DirectX::XMVector3Transform
			(
				base,
				DirectX::XMMatrixRotationZ(longitude_angle * _long)
			);

		DirectX::XMStoreFloat3(&vertices.back().position, vertex);
	}
	
	vector<uint16_t> indices;

	//indices of cone

	for (uint16_t idx = NULL; idx < ndivisions * 3; ++idx)
	{
		indices.push_back(idx + cone_idx);
	}

	//indices of base

	for (uint16_t _long = NULL; _long < ndivisions; ++_long)
	{
		indices.push_back(base_center_idx);
		indices.push_back((1 + _long) % ndivisions + base_edge_idx);
		indices.push_back(_long + base_edge_idx);
	}

	return { std::move(vertices), std::move(indices) };
}
