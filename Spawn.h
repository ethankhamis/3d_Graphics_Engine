#pragma once
#include "Drawable.h"
//#include "BindSolidLongLatSphere.h"
//#include "BindSolidCube.h"
//#include "BindSolidPlane.h"
//#include "PhongShadingCube.h"
//#include "TexturedPlane.h"

#include "Math.cpp"
#include <memory>
#include <concepts>
#include <typeinfo>
#include "Surface.h"
#include "PointLight.h"
#include "imgui/imgui.h"

#include "randomTest.h"
enum struct Geometry;

template<typename T>
concept Geometry_T = requires(T a)
{
	typeid(a) == typeid(Geometry);
};

template<typename T>
concept Switchable = requires(T a)
{
	{a} -> std::convertible_to<int>;
};

struct Spawn
{
	Spawn(Graphics& gfx)
		:
		gfx(gfx),
		light(gfx)
	{}

	enum struct Geometry
	{
		SolidLongLatSphere,
		SolidPlane,
		SolidCube,
		TexturedPlane,
		FullySkinnedCube,
		PhongShadedCube,
		PhongShadedCylinder,
		PhongShadedCone,
		MultiFacedPhongShadedTexturedCube,
		UniFacedPhongShadedTexturedCube,
		PhongShadedMonkey = 11,
		


		LightSource = 10,
		Count,
	};

public:
	void Window(std::vector<std::unique_ptr<Drawable>>& drawables)
	{

		if (ImGui::Begin(""))
		{
			/*
			if (ImGui::Button("Spawn Solid Sphere"))
			{
				drawables.emplace_back(Chosen(Geometry::SolidLongLatSphere));
			}

			if (ImGui::Button("Spawn Solid Plane"))
			{
				drawables.emplace_back(Chosen(Geometry::SolidPlane));
			}

			if (ImGui::Button("Spawn Solid Cube"))
			{
				drawables.emplace_back(Chosen(Geometry::SolidCube));
			}

			if (ImGui::Button("Spawn Textured Plane"))
			{
				drawables.emplace_back(Chosen(Geometry::TexturedPlane));
			}

			if (ImGui::Button("Spawn Fully Textured Cube"))
			{
				drawables.emplace_back(Chosen(Geometry::FullySkinnedCube));
			}

			if (ImGui::Button("Spawn Random Geometry"))
			{
				drawables.emplace_back(Random());
			}
			*/
			if (ImGui::Button("Spawn Phong Shaded Cube"))
			{
				drawables.emplace_back(Chosen(Geometry::PhongShadedCube));
			}
			if (ImGui::Button("Spawn Phong Shaded Cylinder"))
			{
				drawables.emplace_back(Chosen(Geometry::PhongShadedCylinder));
			}
			if (ImGui::Button("Spawn Phong Shaded Cone"))
			{
				drawables.emplace_back(Chosen(Geometry::PhongShadedCone));
			}
			if (ImGui::Button("Spawn Phong Shaded Textured Cube (multi-faced)"))
			{
				drawables.emplace_back(Chosen(Geometry::MultiFacedPhongShadedTexturedCube));
			}
			if (ImGui::Button("Spawn Phong Shaded Textured Cube (uni-faced)"))
			{
				drawables.emplace_back(Chosen(Geometry::UniFacedPhongShadedTexturedCube));
			}
			if (ImGui::Button("Spawn Phong Shaded Monkey"))
			{
				drawables.emplace_back(Chosen(Geometry::PhongShadedMonkey));
			}

		}
		ImGui::End();

	}
public:
	std::unique_ptr<Drawable> Random()
	{
		return Spawner(typedist(rng));
	}

	template<Geometry_T Geometry_t>
	std::unique_ptr<Drawable> Chosen(Geometry_t g)
	{ 
		return Spawner( static_cast<int>(g) );
	}

public:
	PointLight* FetchLight()
	{
		return &light;
	}

private:
	template<Switchable _switch>
		std::unique_ptr<Drawable> Spawner(_switch s)
		{
			const DirectX::XMFLOAT3 matrix = { 1 , NULL , NULL };
			switch (static_cast<int>(s))
			{
				/*

			case static_cast<int>(Geometry::SolidLongLatSphere):
				return std::make_unique<SolidLongLatSphere>(
					gfx,2.0f
					);
				
			case static_cast<int>(Geometry::SolidPlane):
				return std::make_unique<BindSolidPlane>
					(
					gfx, rng, adist, ddist, odist, rdist, hDiv, vDiv
					);
		
			case static_cast<int>(Geometry::SolidCube):
				return std::make_unique<BindSolidCube>
					(
						gfx, rng, adist, ddist, odist, rdist, bdist

						);

			case static_cast<int>(Geometry::TexturedPlane):
				return std::make_unique<TexturedPlane>
					(
					gfx, rng, adist, ddist, odist, rdist
					);
					
			case static_cast<int>(Geometry::FullySkinnedCube):
				return std::make_unique<SkinnedCube>(
					gfx,rng, adist, ddist, odist, rdist
					);
				*/
			case static_cast<int>(Geometry::PhongShadedCube):
				//return std::make_unique<PhongShadingCube>
				//	(gfx, rng, adist, ddist, odist, rdist, bdist,matrix)
				;
					
			case static_cast<int>(Geometry::PhongShadedCylinder):
				//return std::make_unique<Cylinder>(
				//	gfx, rng, adist, ddist, odist,
					//rdist, bdist, tdist)
					;

			case static_cast<int>(Geometry::PhongShadedCone):
				//return std::make_unique<SolidCone>(
				//	gfx, rng, adist, ddist, odist, rdist, tdist
				//	)
					;
					
			case static_cast<int>(Geometry::MultiFacedPhongShadedTexturedCube):
				//return std::make_unique<SkinnedCube>
				//	(gfx, rng, adist, ddist, odist, rdist)
					;

			case static_cast<int>(Geometry::UniFacedPhongShadedTexturedCube):
				//return std::make_unique<SemiSkinnedCube>
					//(gfx, rng, adist, ddist, odist, rdist)
					;

			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
private:
	Graphics& gfx;
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
	std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
	std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
	std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
	std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
	std::uniform_int_distribution<int> latdist{ 5,20 };
	std::uniform_int_distribution<int> longdist{ 10,40 };
	std::uniform_int_distribution<int> vDiv{ 5,20 };
	std::uniform_int_distribution<int> hDiv{ 5,20 };
	std::uniform_real_distribution<float> cdist{ 0.0f, 1.0f };
	std::uniform_int_distribution<int> typedist{ 0, static_cast<int>(Geometry::Count)-6 };
	std::uniform_int_distribution<int> sdist{ 0,2 };
	std::uniform_int_distribution<int> tdist{ 3,30 };
	PointLight light;
	bool lighting = false;
	};