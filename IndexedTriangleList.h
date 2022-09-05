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

	void ApplyNormalsIndependent() noexcept(Debug)
	{
		using namespace DirectX;
		assert(indices.size() % 3 == 0 && indices.size() > 0);
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			auto& v0 = vertices[indices[i]];
			auto& v1 = vertices[indices[i + 1]];
			auto& v2 = vertices[indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.pos);
			const auto p1 = XMLoadFloat3(&v1.pos);
			const auto p2 = XMLoadFloat3(&v2.pos);

			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

			XMStoreFloat3(&v0.n, n);
			XMStoreFloat3(&v1.n, n);
			XMStoreFloat3(&v2.n, n);
		}
	}
	vector<T> vertices;
	vector<uint16_t> indices;
};