#pragma once
#include "Win.h"
#include "Timer.h"
#include "Camera.h"
#include "Spawn.h"
#include "Model.h"
#include "ImGUIManager.h"
#include <set>

struct Application
{
	Application();
	int Start();
	~Application();
private:
	imguiManager imgui;
private:
	Wnd window;
	Timer timer;
	Camera camera;
private:
	Spawn spawn;
	Model nanosuit{ window.grfx(), "Models\\objBuilding.obj" };
private:
	void ExecFrame();
	float speed = 1.f;
};