#pragma once
#include "IndexedTriangleList.h"
#include "DirectXMath.h"
#include <vector>

using std::vector;

struct Cube
{
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create();
};

template<class Vertex>
inline IndexedTriangleList<Vertex> Cube::Create()
{
	constexpr float side = 0.5f;
	vector<DirectX::XMFLOAT3> vertices;
	vertices.emplace_back(-side, -side, -side);
	vertices.emplace_back(side, -side, -side); 
	vertices.emplace_back(-side, side, -side); 
	vertices.emplace_back(side, side, -side);  
	vertices.emplace_back(-side, -side, side); 
	vertices.emplace_back(side, -side, side);  
	vertices.emplace_back(-side, side, side);
	vertices.emplace_back(side, side, side); 
	vector<Vertex> vertexs(vertices.size());
	for (uint32_t idx = 0; idx < vertices.size(); ++idx)
	{
		vertexs[idx].pos = vertices[idx];
	}

	return
	{
		std::move(vertexs),
		{0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	}
	};
}
