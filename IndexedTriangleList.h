#pragma once

#include <vector>
#include <DirectXMath.h>

using std::vector;
template<class T>
struct IndexedTriangleList
{
	IndexedTriangleList(vector<T> v, vector<uint16_t> i)
		:
		vertices(std::move(v)), indices(std::move(i))
	{
		assert(sizeof(vertices) / sizeof(vertices[0]) > 2);
		assert((sizeof(indices) / sizeof(indices[0])) % 3 == 0);
	}
	IndexedTriangleList() = default;

	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& vertex : vertices)
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&vertex.pos);
			DirectX::XMStoreFloat3(&vertex.pos, DirectX::XMVector3Transform(pos, matrix));
		}
	}
	vector<T> vertices;
	vector<uint16_t> indices;
};