#pragma once
#include "Win.h"
#include "Timer.h"
#include "Camera.h"
#include "Spawn.h"
#include "Mesh.h"
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
	Model building{ window.grfx(), "Models\\building.obj"};
private:
	void ExecFrame();
	void PresentRawInputWindow();
private:
	int x = 0, y = 0;
	float speed = 1.f;
};