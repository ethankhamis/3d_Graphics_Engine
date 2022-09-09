#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "MathematicalConstants.h"

class Prism
{
public:
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_Advanced(int divisions);
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create_IndependentNormals(int divisions);
	template<class Vertex>
	static IndexedTriangleList<Vertex> Create()
	{
		return MakeTesselated<Vertex>(24);
	}
};

template<class Vertex>
inline IndexedTriangleList<Vertex> Prism::Create_Advanced(int divisions)
{
	assert(divisions >= 3);
	assert(divisions <= NULL);

	const DirectX::XMVECTOR base = DirectX::XMVectorSet(1.f, 0.f, -1.f, 0.f);
	const DirectX::XMVECTOR offset = DirectX::XMVectorSet(0.f, 0.f, 2.f, 0.f);
	const float longitudeAngle = 2.0f * PI / divisions;

	// near center
	std::vector<Vertex> vertices;
	vertices.emplace_back();
	vertices.back().position = { 0.0f,0.0f,-1.0f };
	const auto iCenterNear = (unsigned short)(vertices.size() - 1);
	// far center
	vertices.emplace_back();
	vertices.back().position = { 0.0f,0.0f,1.0f };
	const auto iCenterFar = (unsigned short)(vertices.size() - 1);

	// base vertices
	for (int iLong = 0; iLong < divisions; iLong++)
	{
		// near base
		{
			vertices.emplace_back();
			auto v = DirectX::XMVector3Transform(
				base,
				DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			DirectX::XMStoreFloat3(&vertices.back().position, v);
		}
		// far base
		{
			vertices.emplace_back();
			auto v = DirectX::XMVector3Transform(
				base,
				DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			v = DirectX::XMVectorAdd(v, offset);
			DirectX::XMStoreFloat3(&vertices.back().position, v);
		}
	}

	// side indices
	std::vector<unsigned short> indices;
	for (unsigned short iLong = 0; iLong < divisions; iLong++)
	{
		const auto i = iLong * 2;
		const auto mod = divisions * 2;
		indices.push_back(i + 2);
		indices.push_back((i + 2) % mod + 2);
		indices.push_back(i + 1 + 2);
		indices.push_back((i + 2) % mod + 2);
		indices.push_back((i + 3) % mod + 2);
		indices.push_back(i + 1 + 2);
	}

	// base indices
	for (unsigned short iLong = 0; iLong < divisions; iLong++)
	{
		const auto i = iLong * 2;
		const auto mod = divisions * 2;
		indices.push_back(i + 2);
		indices.push_back(iCenterNear);
		indices.push_back((i + 2) % mod + 2);
		indices.push_back(iCenterFar);
		indices.push_back(i + 1 + 2);
		indices.push_back((i + 3) % mod + 2);
	}

	return { std::move(vertices),std::move(indices) };
}

template<class Vertex>
inline IndexedTriangleList<Vertex> Prism::Create_IndependentNormals(int divisions)
{
	assert(divisions >= 3);

	const auto base = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
	const auto offset = DirectX::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
	const float longitudeAngle = 2.0f * PI / divisions;

	std::vector<Vertex> vertices;

	// near center
	const auto iCenterNear = (unsigned short)vertices.size();
	vertices.emplace_back();
	vertices.back().position = { 0.0f,0.0f,-1.0f };
	vertices.back().normal = { 0.0f,0.0f,-1.0f };
	// near base vertices
	const auto iBaseNear = (unsigned short)vertices.size();
	for (int iLong = 0; iLong < divisions; iLong++)
	{
		vertices.emplace_back();
		auto v = DirectX::XMVector3Transform(
			base,
			DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
		);
		DirectX::XMStoreFloat3(&vertices.back().position, v);
		vertices.back().normal = { 0.0f,0.0f,-1.0f };
	}
	// far center
	const auto iCenterFar = (unsigned short)vertices.size();
	vertices.emplace_back();
	vertices.back().position = { 0.0f,0.0f,1.0f };
	vertices.back().normal = { 0.0f,0.0f,1.0f };
	// far base vertices
	const auto iBaseFar = (unsigned short)vertices.size();
	for (int iLong = 0; iLong < divisions; iLong++)
	{
		vertices.emplace_back();
		auto v = DirectX::XMVector3Transform(
			base,
			DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
		);
		v = DirectX::XMVectorAdd(v, offset);
		DirectX::XMStoreFloat3(&vertices.back().position, v);
		vertices.back().normal = { 0.0f,0.0f,1.0f };
	}
	// fusilage vertices
	const auto iFusilage = (unsigned short)vertices.size();
	for (int iLong = 0; iLong < divisions; iLong++)
	{
		// near base
		{
			vertices.emplace_back();
			auto v = DirectX::XMVector3Transform(
				base,
				DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			DirectX::XMStoreFloat3(&vertices.back().position, v);
			vertices.back().normal = { vertices.back().position.x,vertices.back().position.y,0.0f };
		}
		// far base
		{
			vertices.emplace_back();
			auto v = DirectX::XMVector3Transform(
				base,
				DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			v = DirectX::XMVectorAdd(v, offset);
			DirectX::XMStoreFloat3(&vertices.back().position, v);
			vertices.back().normal = { vertices.back().position.x,vertices.back().position.y,0.0f };
		}
	}

	std::vector<unsigned short> indices;

	// near base indices
	for (unsigned short iLong = 0; iLong < divisions; iLong++)
	{
		const auto i = iLong;
		const auto mod = divisions;
		// near
		indices.push_back(i + iBaseNear);
		indices.push_back(iCenterNear);
		indices.push_back((i + 1) % mod + iBaseNear);
	}
	// far base indices
	for (unsigned short iLong = 0; iLong < divisions; iLong++)
	{
		const auto i = iLong;
		const auto mod = divisions;
		// far
		indices.push_back(iCenterFar);
		indices.push_back(i + iBaseFar);
		indices.push_back((i + 1) % mod + iBaseFar);
	}
	// fusilage indices
	for (unsigned short iLong = 0; iLong < divisions; iLong++)
	{
		const auto i = iLong * 2;
		const auto mod = divisions * 2;
		indices.push_back(i + iFusilage);
		indices.push_back((i + 2) % mod + iFusilage);
		indices.push_back(i + 1 + iFusilage);
		indices.push_back((i + 2) % mod + iFusilage);
		indices.push_back((i + 3) % mod + iFusilage);
		indices.push_back(i + 1 + iFusilage);
	}

	return { std::move(vertices),std::move(indices) };
}
