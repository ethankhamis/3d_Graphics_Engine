#pragma once
#include "Win.h"
#include "Timer.h"

struct Application
{
	Application();
	int Start();
	~Application();
private:
	Wnd window;
	Timer timer;
private:
	void ExecFrame();
private:
	std::vector<std::unique_ptr<struct Drawable>> drawables;
	static constexpr unsigned int nDrawables = 1;
};