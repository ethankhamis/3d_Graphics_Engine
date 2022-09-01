#include "BindCube.h"
#include "DefaultBindables.h"
#include "ThrowMacros.h"
#include "Cube.h"
#include <algorithm>

BindCube::BindCube(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	if (!is_static_init())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
		};
		auto model = Cube::Create<Vertex>();
		model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.2f));
		ApplyStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"ColourIndexVS.cso");
		auto pvsbc = pvs->FetchByteCode();
		ApplyStaticBind(std::move(pvs));

		ApplyStaticBind(std::make_unique<PixelShader>(gfx, L"ColourIndexPS.cso"));

		ApplyStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		struct PSConsts
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PSConsts PSC2 =
		{
			{
				{ 1.0f,0.0f,0.0f },
				{ 0.5f,0.0f,0.0f  },
				{ 0.5f,0.0f,0.0f  },
				{ 1.0f,0.0f,0.0f  },
				{ 0.5f,0.0f,0.0f  },
				{ 1.0f,0.0f,0.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.5f,0.0f,0.0f  },
			}
		};
		ApplyStaticBind(std::make_unique<PixelConstantBuffer<PSConsts>>(gfx, PSC2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		ApplyStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		ApplyStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		ApplyIndexFromStatic();
	}

	ApplyBind(std::make_unique<TransformConstBuffer>(gfx, *this));

	//model deformation transform per instance
	DirectX::XMStoreFloat3x3
	(
		&model_transform,
		DirectX::XMMatrixScaling
		(
			1.0f,
			1.0f,
			bdist(rng)
		)
	);

}

void BindCube::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX BindCube::FetchTransformMat() const noexcept
{
	return DirectX::XMLoadFloat3x3(&model_transform) *
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
