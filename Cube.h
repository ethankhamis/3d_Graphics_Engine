#pragma once
#include "IndexedTriangleList.h"
#include "DirectXMath.h"
#include <vector>

using std::vector;

struct Cube
{
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create(float _side = .5f);
	
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_Skinned(float _side = 0.5f);

	//template<class Vertex>
	//static IndexedTriangleList<Vertex> Create_SemiSkinned();

	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_Independentf(float _side = 0.5f);

	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_Skinned_Independentf(float _side = .5f);
};

template<class Vertex>
inline IndexedTriangleList<Vertex> Cube::Create(float _side)
{
	float side = _side;
	vector<Vertex> vertices(8);
	vertices[0].position = { -side, -side, -side };
	vertices[1].position = { side, -side, -side };
	vertices[2].position = { -side, side, -side };
	vertices[3].position = { side, side, -side };
	vertices[4].position = { -side, -side, side };
	vertices[5].position = { side, -side, side };
	vertices[6].position = { -side, side, side };
	vertices[7].position = { side, side, side };

	return
	{
		std::move(vertices),
		{0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	}
	};
}

template<class Vertex>
inline IndexedTriangleList<Vertex> Cube::Create_Skinned(float _side)
{

	float side = _side;

	std::vector<Vertex> vertices(14);

	vertices[0].position = { -side,-side,-side };
	vertices[0].tex = { 2.0f / 3.0f,0.0f / 4.0f };
	vertices[1].position = { side,-side,-side };
	vertices[1].tex = { 1.0f / 3.0f,0.0f / 4.0f };
	vertices[2].position = { -side,side,-side };
	vertices[2].tex = { 2.0f / 3.0f,1.0f / 4.0f };
	vertices[3].position = { side,side,-side };
	vertices[3].tex = { 1.0f / 3.0f,1.0f / 4.0f };
	vertices[4].position = { -side,-side,side };
	vertices[4].tex = { 2.0f / 3.0f,3.0f / 4.0f };
	vertices[5].position = { side,-side,side };
	vertices[5].tex = { 1.0f / 3.0f,3.0f / 4.0f };
	vertices[6].position = { -side,side,side };
	vertices[6].tex = { 2.0f / 3.0f,2.0f / 4.0f };
	vertices[7].position = { side,side,side };
	vertices[7].tex = { 1.0f / 3.0f,2.0f / 4.0f };
	vertices[8].position = { -side,-side,-side };
	vertices[8].tex = { 2.0f / 3.0f,4.0f / 4.0f };
	vertices[9].position = { side,-side,-side };
	vertices[9].tex = { 1.0f / 3.0f,4.0f / 4.0f };
	vertices[10].position = { -side,-side,-side };
	vertices[10].tex = { 3.0f / 3.0f,1.0f / 4.0f };
	vertices[11].position = { -side,-side,side };
	vertices[11].tex = { 3.0f / 3.0f,2.0f / 4.0f };
	vertices[12].position = { side,-side,-side };
	vertices[12].tex = { 0.0f / 3.0f,1.0f / 4.0f };
	vertices[13].position = { side,-side,side };
	vertices[13].tex = { 0.0f / 3.0f,2.0f / 4.0f };

	return{
		std::move(vertices),{
			0,2,1,   2,3,1,
			4,8,5,   5,8,9,
			2,6,3,   3,6,7,
			4,5,7,   4,7,6,
			2,10,11, 2,11,6,
			12,3,7,  12,7,13
		}
	};
}

template<class Vertex>
inline IndexedTriangleList<Vertex> Cube::Create_Independentf(float _side)
{
	float side = _side;

	std::vector<Vertex> vertices(24);
	vertices[0].position = { -side,-side,-side };// 0 near side
	vertices[1].position = { side,-side,-side };// 1
	vertices[2].position = { -side,side,-side };// 2
	vertices[3].position = { side,side,-side };// 3
	vertices[4].position = { -side,-side,side };// 4 far side
	vertices[5].position = { side,-side,side };// 5
	vertices[6].position = { -side,side,side };// 6
	vertices[7].position = { side,side,side };// 7
	vertices[8].position = { -side,-side,-side };// 8 left side
	vertices[9].position = { -side,side,-side };// 9
	vertices[10].position = { -side,-side,side };// 10
	vertices[11].position = { -side,side,side };// 11
	vertices[12].position = { side,-side,-side };// 12 right side
	vertices[13].position = { side,side,-side };// 13
	vertices[14].position = { side,-side,side };// 14
	vertices[15].position = { side,side,side };// 15
	vertices[16].position = { -side,-side,-side };// 16 bottom side
	vertices[17].position = { side,-side,-side };// 17
	vertices[18].position = { -side,-side,side };// 18
	vertices[19].position = { side,-side,side };// 19
	vertices[20].position = { -side,side,-side };// 20 top side
	vertices[21].position = { side,side,-side };// 21
	vertices[22].position = { -side,side,side };// 22
	vertices[23].position = { side,side,side };// 23

	return{
		std::move(vertices),{
			0,2, 1,    2,3,1,
			4,5, 7,    4,7,6,
			8,10, 9,  10,11,9,
			12,13,15, 12,15,14,
			16,17,18, 18,17,19,
			20,23,21, 20,22,23
		}
	};
}

template<class Vertex>
inline IndexedTriangleList<Vertex> Cube::Create_Skinned_Independentf(float _side)
{
	auto IndexedTriangleList_ = Create_Independentf<Vertex>();

	IndexedTriangleList_.vertices[0]. tc = { 0.f,0.f };
	IndexedTriangleList_.vertices[1]. tc = { 1.f,0.f };
	IndexedTriangleList_.vertices[2]. tc = { 0.f,1.f };
	IndexedTriangleList_.vertices[3]. tc = { 1.f,1.f };
	IndexedTriangleList_.vertices[4]. tc = { 0.f,0.f };
	IndexedTriangleList_.vertices[5]. tc = { 1.f,0.f };
	IndexedTriangleList_.vertices[6]. tc = { 0.f,1.f };
	IndexedTriangleList_.vertices[7]. tc = { 1.f,1.f };
	IndexedTriangleList_.vertices[8]. tc = { 0.f,0.f };
	IndexedTriangleList_.vertices[9]. tc = { 1.f,0.f };
	IndexedTriangleList_.vertices[10].tc = { 0.f,1.f };
	IndexedTriangleList_.vertices[11].tc = { 1.f,1.f };
	IndexedTriangleList_.vertices[12].tc = { 0.f,0.f };
	IndexedTriangleList_.vertices[13].tc = { 1.f,0.f };
	IndexedTriangleList_.vertices[14].tc = { 0.f,1.f };
	IndexedTriangleList_.vertices[15].tc = { 1.f,1.f };
	IndexedTriangleList_.vertices[16].tc = { 0.f,0.f };
	IndexedTriangleList_.vertices[17].tc = { 1.f,0.f };
	IndexedTriangleList_.vertices[18].tc = { 0.f,1.f };
	IndexedTriangleList_.vertices[19].tc = { 1.f,1.f };
	IndexedTriangleList_.vertices[20].tc = { 0.f,0.f };
	IndexedTriangleList_.vertices[21].tc = { 1.f,0.f };
	IndexedTriangleList_.vertices[22].tc = { 0.f,1.f };
	IndexedTriangleList_.vertices[23].tc = { 1.f,1.f };

	return IndexedTriangleList_;
}

/*
template<class Vertex>
inline IndexedTriangleList<Vertex> Cube::Create_SemiSkinned()
{

	constexpr float side = 0.5f;


	vector<Vertex> vertices(8);
	vertices[0].position = { -side, -side, -side };
	vertices[0].tex = { 0.0f,0.0f };
	vertices[1].position = { side, -side, -side };
	vertices[1].tex = { 1.0f,0.0f };
	vertices[2].position = { -side, side, -side };
	vertices[2].tex = { 0.0f,1.0f };
	vertices[3].position = { side, side, -side };
	vertices[3].tex = { 1.0f,1.0f };
	vertices[4].position = { -side, -side, side };
	vertices[4].tex = { 0.0f,0.0f };
	vertices[5].position = { side, -side, side };
	vertices[5].tex = { 1.0f,0.0f };
	vertices[6].position = { -side, side, side };
	vertices[6].tex = { 0.0f,1.0f };
	vertices[7].position = { side, side, side };
	vertices[7].tex = { 1.0f,1.0f };





	return{
		std::move(vertices),{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4}};
	}
	*/