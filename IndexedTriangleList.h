#pragma once

#include <vector>
#include <DirectXMath.h>

using std::vector;
template<class T>
struct IndexedTriangleList
{
	IndexedTriangleList() = default;
	IndexedTriangleList(vector<T> v, vector<uint16_t> i)
		:
		vertices(std::move(v)), indices(std::move(i))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

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