#pragma once
#include "IndexedTriangleList.h"
#include "DynamicVertex.h"
#include "DirectXMath.h"
#include <vector>

using std::vector;

struct Cube
{

	static IndexedTriangleList Create_Independentf(DynamicVertex::VertexLayout vlayout);

	static IndexedTriangleList Create_Skinned_Independentf();
};


inline IndexedTriangleList Cube::Create_Independentf(DynamicVertex::VertexLayout vlayout)
{
	float side = 0.5f;

	DynamicVertex::VertexBuffer vertices(std::move(vlayout), 24u);

	vertices[0].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = {-side,-side,-side};//near face
	vertices[1].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,-side,-side };
	vertices[2].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,side,-side };
	vertices[3].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,side,-side };
	vertices[4].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,-side,side };//furthest face
	vertices[5].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,-side,side };
	vertices[6].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,side,side };
	vertices[7].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,side,side };
	vertices[8].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,-side,-side };//left face
	vertices[9].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,side,-side };
	vertices[10].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,-side,side };
	vertices[11].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,side,side };
	vertices[12].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,-side,-side };//right face
	vertices[13].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,side,-side };
	vertices[14].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,-side,side };
	vertices[15].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,side,side };
	vertices[16].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,-side,-side };//bottom face
	vertices[17].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,-side,-side };
	vertices[18].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,-side,side };
	vertices[19].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,-side,side };
	vertices[20].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,side,-side };//top face
	vertices[21].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,side,-side };
	vertices[22].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { -side,side,side };
	vertices[23].Attribute<DynamicVertex::VertexLayout::MemberType::Position3D>() = { side,side,side };

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



inline IndexedTriangleList Cube::Create_Skinned_Independentf()
{
	IndexedTriangleList IndexedTriangleList_ = Create_Independentf(std::move(DynamicVertex::VertexLayout{}
		.Emplace_Back(DynamicVertex::VertexLayout::MemberType::Position3D)
		.Emplace_Back(DynamicVertex::VertexLayout::MemberType::Normal)
		.Emplace_Back(DynamicVertex::VertexLayout::MemberType::Texture2D)
	));

	IndexedTriangleList_.vertices[0].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {0.0f,0.0f};
	IndexedTriangleList_.vertices[1].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {1.0f,0.0f};
	IndexedTriangleList_.vertices[2].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {0.0f,1.0f};
	IndexedTriangleList_.vertices[3].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {1.0f,1.0f};
	IndexedTriangleList_.vertices[4].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {0.0f,0.0f};
	IndexedTriangleList_.vertices[5].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {1.0f,0.0f};
	IndexedTriangleList_.vertices[6].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {0.0f,1.0f};
	IndexedTriangleList_.vertices[7].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {1.0f,1.0f};
	IndexedTriangleList_.vertices[8].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {0.0f,0.0f};
	IndexedTriangleList_.vertices[9].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() =  {1.0f,0.0f};
	IndexedTriangleList_.vertices[10].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {0.0f,1.0f};
	IndexedTriangleList_.vertices[11].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {1.0f,1.0f};
	IndexedTriangleList_.vertices[12].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {0.0f,0.0f};
	IndexedTriangleList_.vertices[13].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {1.0f,0.0f};
	IndexedTriangleList_.vertices[14].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {0.0f,1.0f};
	IndexedTriangleList_.vertices[15].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {1.0f,1.0f};
	IndexedTriangleList_.vertices[16].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {0.0f,0.0f};
	IndexedTriangleList_.vertices[17].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {1.0f,0.0f};
	IndexedTriangleList_.vertices[18].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {0.0f,1.0f};
	IndexedTriangleList_.vertices[19].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {1.0f,1.0f};
	IndexedTriangleList_.vertices[20].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {0.0f,0.0f};
	IndexedTriangleList_.vertices[21].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {1.0f,0.0f};
	IndexedTriangleList_.vertices[22].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {0.0f,1.0f};
	IndexedTriangleList_.vertices[23].Attribute<DynamicVertex::VertexLayout::MemberType::Texture2D>() = {1.0f,1.0f};

	return IndexedTriangleList_;
}
