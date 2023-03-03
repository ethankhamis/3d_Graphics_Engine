#pragma once
#include "isRendered.h"
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



struct Spawn
{
	Spawn(Graphics& gfx)
		:
		gfx(gfx),
		light(gfx)
	{
	}

public:
	/*
	void Window(std::vector<std::unique_ptr<Model>>& models)
	{

		if (ImGui::Begin(""))
		{
			ImGui::InputText("Add Model", data, 50, 0);
			if (ImGui::Button("Search Directory"))
			{
			}

		}
		ImGui::End();

	}
	*/
private:

public:
	PointLight* FetchLight()
	{
		return &light;
	}

private:
	//char data[50];
	Graphics& gfx;
	PointLight light;
	bool lighting = false;
	};