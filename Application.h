#pragma once
#include "Win.h"
#include "Timer.h"

struct Application
{
private:
	Wnd window;
	Timer timer;
private:
	void ExecFrame();
public:
	Application();
	int Start();
};