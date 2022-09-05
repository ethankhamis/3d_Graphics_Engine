#pragma once
#include "Win.h"
#include "Timer.h"
#include "Camera.h"
#include "Spawn.h"
#include "ImGUIManager.h"

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
private:
	void ExecFrame();
private:
	std::vector<std::unique_ptr<struct Drawable>> drawables;
	static constexpr unsigned int nDrawables = 0;
	float speed = 1.f;
private:
	bool show_demo = true;
};