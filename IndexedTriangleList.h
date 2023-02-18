#pragma once

#include <vector>
#include <DirectXMath.h>
#include "DynamicVertex.h"

struct IndexedTriangleList
{
	IndexedTriangleList() = default;
	IndexedTriangleList(DynamicVertex::VertexBuffer v, std::vector<uint16_t> i) // vertices + indices
		:
		vertices(std::move(v)), indices(std::move(i))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}

	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (int idx = NULL; idx < vertices.size(); idx++)
		{
			auto& position = vertices[idx].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>();
			DirectX::XMStoreFloat3(&position,DirectX::XMVector3Transform( DirectX::XMLoadFloat3(&position), matrix));
		}
	}

	void ApplyNormalsIndependent() noexcept_unless
	{
		using namespace DirectX;
		assert(indices.size() % 3 == 0 && indices.size() > 0);
		for (size_t idx = 0; idx < indices.size(); idx += 3)
		{
			DynamicVertex::Vertex v0 = vertices[indices[idx]];
			DynamicVertex::Vertex v1 = vertices[indices[idx + 1]];
			DynamicVertex::Vertex v2 = vertices[indices[idx + 2]];
			const vector p0 = XMLoadFloat3(&v0.Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>());
		const vector p1 = XMLoadFloat3(&v1.Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>());
			const vector p2 = XMLoadFloat3(&v2.Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>());

		const vector normal = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));
		
					XMStoreFloat3(&v0.Attribute<DynamicVertex::VertexLayout::MemberType::Normal>(), normal);
					XMStoreFloat3(&v1.Attribute<DynamicVertex::VertexLayout::MemberType::Normal>(), normal);
					XMStoreFloat3(&v2.Attribute<DynamicVertex::VertexLayout::MemberType::Normal>(), normal);
	}
}
	DynamicVertex::VertexBuffer vertices;
	std::vector<uint16_t> indices;
};