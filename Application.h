#pragma once
#include "Win.h"
#include "Timer.h"
#include "Camera.h"
#include "Spawn.h"
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
private:
	void ExecFrame();
private:
	void SpawnSimulationWindow() noexcept;
	void SpawnCubeWindowManagerWindow() noexcept;
	void SpawnCubeWindows();
private:
	std::vector<std::unique_ptr<struct Drawable>> drawables;
	std::vector<struct PhongShadingCube*> cubes;
private:
	static constexpr unsigned int nDrawables = 50;
	float speed = 1.f;
private:
	std::optional<int> comboCubeIndex;
	std::set<int> cubeControlIds;
};