#pragma once
#include "Win.h"
#include "Timer.h"
#include "Camera.h"
#include "Spawn.h"
#include "Mesh.h"
#include "BindTexturedPlane.h"
#include "ImGUIManager.h"
#include <set>
#include "BindTexturedCube.h"

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
	Model goblin{ window.grfx(),"Models\\abandoned\\rOOM.obj" };
	//Model building{ window.grfx(), "Models\\nanosuit_textured\\nanosuit.obj"};
	//Model wall{window.grfx(), "Models\\brickwall\\brickwall.obj"};
private:
	void ExecFrame();
private:
	int x = 0, y = 0;
	float speed = 1.f;
};