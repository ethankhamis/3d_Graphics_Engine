#include "Application.h"
#include <sstream>
#include "BindCube.h"
#include <memory>
#include "BindLongLatSphere.h"
#include "Timer.h"
#include "MathematicalConstants.h"
#include <DirectXMath.h>
#include "BindPlane.h"
#include <algorithm>

Application::Application()
	:window(1000, 750, L"6~3D")
{
	/*
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
			case 0:
				return std::make_unique<BindCube>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 1:
				return std::make_unique<LongLatSphere>(
					gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
					);
			case 2:
				return std::make_unique<Plane_t>
					(
						gfx, rng, adist, ddist,
						odist, rdist, hDiv, vDiv
					);

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
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };

		std::uniform_int_distribution<int> vDiv{ 5,20 };
		std::uniform_int_distribution<int> hDiv{ 5,20 };

		std::uniform_int_distribution<int> typedist{ 0,2 };
	};

	Factory f(window.grfx());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);

	*/
	window.grfx().ApplyProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void Application::ExecFrame()
{
	auto delta = timer.MarkTime();
	window.grfx().ClearBuffer(0.01f, 0.0f, 0.0f);
	for (auto& drawable : drawables)
	{
		drawable->Update(delta);
		drawable->Render(window.grfx());
	}
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
