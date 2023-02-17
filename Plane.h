#pragma once

#include <vector>
#include <array>
#include "Math.cpp"
#include "IndexedTriangleList.h"
#include "DynamicVertex.h"

using std::vector;
using std::array;

struct Plane
{
	static IndexedTriangleList Create_Advanced_Textured(DynamicVertex::VertexLayout vlayout,uint16_t divisions_horizontal, uint16_t divisions_vertical);
	static IndexedTriangleList Create_Textured()
	{
		DynamicVertex::VertexLayout vlayout;
		vlayout.Append(DynamicVertex::VertexLayout::Position3D);
		vlayout.Append(DynamicVertex::VertexLayout::Normal);
		vlayout.Append(DynamicVertex::VertexLayout::Texture2D);
		return Create_Advanced_Textured(std::move(vlayout), 1, 1);
	}
};

inline IndexedTriangleList Plane::Create_Advanced_Textured(DynamicVertex::VertexLayout vlayout,uint16_t divisions_horizontal, uint16_t divisions_vertical)
{
	assert(divisions_horizontal >= 1);
	assert(divisions_vertical >= 1);

	// define the number of horizontal and vertical tile's
	const int nHorizontal = 1 + divisions_horizontal;
	const int nVertical = 1 + divisions_vertical;
	//calculate total size
	const unsigned short int size = nHorizontal * nVertical;
	// store the vertex layout within a new type
	DynamicVertex::VertexBuffer vertices{
						std::move(vlayout)
										};
	// set dimensions for width and height
	constexpr float width = 2.0f;
	constexpr float height = 2.0f;

	{
		//calculate the sides and division total for 2d axis'
		const float side_y = height / 2.0f;
		const float sizeof_division_y = height / static_cast<float>(divisions_vertical);
		const float side_x = width / 2.0f;
		const float sizeof_division_x = width / static_cast<float>(divisions_horizontal);
		const float sizeof_tex_division_x = 1.0f / static_cast<float>(divisions_horizontal);
		const float sizeof_tex_division_y = 1.0f / static_cast<float>(divisions_vertical);

		//find the position of th bottom left of the plane
		const DirectX::XMVECTOR bottomL = DirectX::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);

		//nested loop through both the vertical and horizontal axis'
		for (unsigned int ver = 0, idx = 0; ver < nVertical; ++ver) // On^2 
		{// initialise the position of the texture and position based on the current index
			const float vertical_pos = static_cast<float>(ver) * sizeof_division_y;
			const float vertical_pos_tex = 1.0f - static_cast<float>(ver) * sizeof_tex_division_y;

			for (unsigned int hor = 0; hor < nHorizontal; hor++, ++idx)
			{
				// initialise the position of the texture and position based on the current index
				const float horizontal_pos = static_cast<float>(hor) * sizeof_division_x;
				const float horizontal_tex_pos = static_cast<float>(hor) * sizeof_tex_division_x;
				// emplacee vertex position, normal map and texture map
				vertices.Emplace_Back
				(
					float3{ horizontal_pos, vertical_pos, 0.f },
					float3{ 0.f,0.f,-1.f },
					float2{horizontal_tex_pos, vertical_pos_tex}
				);
			}
		}
	}

	vector<uint16_t> indices;
	indices.reserve(Sqrd(size) * 6);
	{
		//lambda function to find indicices when given x and y values
		const auto find_idx = [nHorizontal](unsigned int hor, unsigned int ver)
		{
			return static_cast<uint16_t>(ver * nHorizontal + hor);
		};
		//nested loop O(n^2)
		for (uint32_t y = NULL; y < nVertical-1; ++y)
		{
			for (uint32_t x = NULL; x < nHorizontal-1; ++x)
			{
				//create array of four and store them as the 
				//permutations of 0-1 passed into the lamda function
				const std::array<uint16_t, 4> IdxArr =
				{ find_idx(x,y), find_idx(x + 1,y), find_idx(x, y + 1), find_idx(x + 1,y + 1) }; // permutations

				//emplaces calculated indices
				indices.emplace_back(IdxArr[0]);
				indices.emplace_back(IdxArr[2]);
				indices.emplace_back(IdxArr[1]);
				indices.emplace_back(IdxArr[1]);
				indices.emplace_back(IdxArr[2]);
				indices.emplace_back(IdxArr[3]);
			}
		}
	}
	//return the vertices and indicies
	return { std::move(vertices), std::move(indices) };
}
