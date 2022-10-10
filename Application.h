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
	Model building{ window.grfx(), "Models\\objBuilding.obj" };
private:
	void ExecFrame();
	void PresentModelWnd();

	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;

	float speed = 1.f;
};