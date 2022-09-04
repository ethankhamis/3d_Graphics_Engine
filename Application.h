#pragma once
#include "Win.h"
#include "Timer.h"
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
private:
	void ExecFrame();
private:
	std::vector<std::unique_ptr<struct Drawable>> drawables;
	static constexpr unsigned int nDrawables = 60;
	float speed = 1.f;
private:
	bool show_demo = true;
};