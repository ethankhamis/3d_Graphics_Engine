#pragma once

#include <vector>
#include <array>
#include "MathematicalConstants.h"
#include "IndexedTriangleList.h"

using std::vector;
using std::array;

struct Plane
{
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_Advanced(uint16_t divisions_horizontal, uint16_t divisions_vertical);
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create()
	{
		return Create_Advanced<Vertex>(1, 1);
	}
};

template<class Vertex>
inline IndexedTriangleList<Vertex> Plane::Create_Advanced(uint16_t divisions_horizontal, uint16_t divisions_vertical)
{
	assert(divisions_horizontal >= 1);
	assert(divisions_vertical >= 1);

	const int nHorizontal = 1 + divisions_horizontal;
	const int nVertical = 1 + divisions_vertical;
	const unsigned short int size = nHorizontal * nVertical;
	vector<Vertex> vertices(size);
	constexpr float width = 2.0f;
	constexpr float height = 2.0f;

	{
		const float side_y = height / 2.0f;
		const float sizeof_division_y = height / static_cast<float>(divisions_vertical);
		const float side_x = width / 2.0f;
		const float sizeof_division_x = width / static_cast<float>(divisions_horizontal);

		const DirectX::XMVECTOR bottomL = DirectX::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);

		for (unsigned int ver = 0, idx = 0; ver < nVertical; ++ver) // On^2 
		{
			const float vertical_pos = static_cast<float>(ver) * sizeof_division_y;

			for (unsigned int hor = 0; hor < nHorizontal; hor++, ++idx)
			{
				const DirectX::XMVECTOR vertex = DirectX::XMVectorAdd
				(
					bottomL,
					DirectX::XMVectorSet(static_cast<float>(hor) * sizeof_division_x,
						vertical_pos, 0.0f, 0.0f)
				);
				DirectX::XMStoreFloat3(&vertices[idx].pos, vertex);
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
				{ find_idx(x,y), find_idx(x + 1,y), find_idx(x, y + 1), find_idx(x + 1,y + 1) };
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
