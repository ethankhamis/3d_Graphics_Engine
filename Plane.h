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

	const int nHorizontal = 1 + divisions_horizontal;
	const int nVertical = 1 + divisions_vertical;
	const unsigned short int size = nHorizontal * nVertical;
	
	DynamicVertex::VertexBuffer vertices{
						std::move(vlayout)
										};

	constexpr float width = 2.0f;
	constexpr float height = 2.0f;

	{
		const float side_y = height / 2.0f;
		const float sizeof_division_y = height / static_cast<float>(divisions_vertical);
		const float side_x = width / 2.0f;
		const float sizeof_division_x = width / static_cast<float>(divisions_horizontal);
		const float sizeof_tex_division_x = 1.0f / static_cast<float>(divisions_horizontal);
		const float sizeof_tex_division_y = 1.0f / static_cast<float>(divisions_vertical);

		const DirectX::XMVECTOR bottomL = DirectX::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);

		for (unsigned int ver = 0, idx = 0; ver < nVertical; ++ver) // On^2 
		{
			const float vertical_pos = static_cast<float>(ver) * sizeof_division_y;
			const float vertical_pos_tex = 1.0f - static_cast<float>(ver) * sizeof_tex_division_y;

			for (unsigned int hor = 0; hor < nHorizontal; hor++, ++idx)
			{
				const float horizontal_pos = static_cast<float>(hor) * sizeof_division_x;
				const float horizontal_tex_pos = static_cast<float>(hor) * sizeof_tex_division_x;
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
		const auto find_idx = [nHorizontal](unsigned int hor, unsigned int ver)
		{
			return static_cast<uint16_t>(ver * nHorizontal + hor);
		};

		for (uint32_t y = NULL; y < nVertical-1; ++y)
		{
			for (uint32_t x = NULL; x < nHorizontal-1; ++x)
			{
				const std::array<uint16_t, 4> IdxArr =
				{ find_idx(x,y), find_idx(x + 1,y), find_idx(x, y + 1), find_idx(x + 1,y + 1) }; // permutations
				indices.emplace_back(IdxArr[0]);
				indices.emplace_back(IdxArr[2]);
				indices.emplace_back(IdxArr[1]);
				indices.emplace_back(IdxArr[1]);
				indices.emplace_back(IdxArr[2]);
				indices.emplace_back(IdxArr[3]);
			}
		}
	}
	return { std::move(vertices), std::move(indices) };
}
