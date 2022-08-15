#pragma once
#include "Win.h"
#include "Timer.h"

struct Application
{
	Application();
	int Start();
private:
	Wnd window;
	Timer timer;
private:
	void ExecFrame();
};