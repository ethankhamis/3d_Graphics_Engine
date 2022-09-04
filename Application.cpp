#include "Application.h"
#include <sstream>
#include "BindSolidCube.h"
#include <memory>
#include "BindLongLatSphere.h"
#include "Timer.h"
#include "MathematicalConstants.h"
#include <DirectXMath.h>
#include "BindSolidPlane.h"
#include <algorithm>
#include "GfxDeviceInterface+Mng.h"
#include "Surface.h"
#include "TexturedPlane.h"
//#include "SemiSkinnedCube.h"
#include "SkinnedCube.h"

#include "imgui/imgui.h"


GDIpManager gdipm;

Application::Application()
	:window(1000, 750, L"6~3D")
{
	
	struct Factory
	{
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}

		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
				/*
			case 0:
				return std::make_unique<BindSolidCube>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);*/
			case 0:
				return std::make_unique<LongLatSphere>(
					gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
					);

				/*
			case 1:
				return std::make_unique<BindSolidPlane>
					(
					gfx, rng, adist, ddist, odist, rdist,hDiv,vDiv
					);
		*/
			case 1:
				return std::make_unique<BindSolidCube>
					(
						gfx, rng, adist, ddist, odist, rdist, bdist

						);
				/*
			case 1:
				return std::make_unique<Plane_t>
					(
						gfx, rng, adist, ddist,
						odist, rdist, hDiv, vDiv
						);
				*/

			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}


	private:
		Graphics& gfx;

		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 10,20 };
		std::uniform_int_distribution<int> longdist{ 10,20 };

		std::uniform_int_distribution<int> vDiv{ 5,20 };
		std::uniform_int_distribution<int> hDiv{ 5,20 };

		std::uniform_int_distribution<int> typedist{ 0,1 };
	};

	Factory f(window.grfx());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, Factory{ window.grfx() });
	
	window.grfx().ApplyProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void Application::ExecFrame()
{
	auto delta = timer.MarkTime() * speed;

	window.grfx().ClearBuffer(0.01f, 0.0f, 0.0f);
	for (auto& drawable : drawables)
	{
		drawable->Update(window.kbd.Key_Pressed(VK_SPACE) ? 0.0f : delta);
		drawable->Render(window.grfx());
	}

	if (ImGui::Begin("Time"))
	{
		ImGui::SliderFloat("Scale Speed", &speed, 0.f, 4.f);
	}

	ImGui::End();

	window.grfx().EndFrame();
}

Application::~Application() {}

int Application::Start()
{
	while (1)
	{
		if (const std::optional<int>(exceptionCode) = Wnd::Messages())
		{
			return *exceptionCode;
		}

		ExecFrame();
	}
}
