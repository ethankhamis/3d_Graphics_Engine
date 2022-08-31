#pragma once
#include "MathematicalConstants.h"
#include <DirectXMath.h>
#include "IndexedTriangleList.h"

using std::vector;

struct Cone
{
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_Advanced(int divisions);
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create()
	{
		return Create_Advanced<Vertex>(24);
	}
};

template<class Vertex>
inline IndexedTriangleList<Vertex> Cone::Create_Advanced(int divisions)
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
