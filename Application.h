#pragma once
#include "Win.h"

struct Application
{
private:
	Wnd window;
	void ExecFrame();
public:
	Application();
	int Start();
};