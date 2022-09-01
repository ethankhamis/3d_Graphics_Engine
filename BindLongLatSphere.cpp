#include "BindLongLatSphere.h"
#include "DefaultBindables.h"
#include "ThrowMacros.h"
#include "LongLatSphere.h"

LongLatSphere::LongLatSphere(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_int_distribution<int>& longdist, std::uniform_int_distribution<int>& latdist)
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
		auto pvs = std::make_unique<VertexShader>(gfx, L"ColourIndexVS.cso");
		auto pvsbc = pvs->FetchByteCode();
		ApplyStaticBind(std::move(pvs));
		ApplyStaticBind(std::make_unique<PixelShader>(gfx, L"ColourIndexPS.cso"));

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PixelShaderConstants PSC2 =
		{
			{
				{ 1.0f,0.0f,0.0f },
				{ 1.0f,0.0f,0.0f },
				{ 1.0f,0.0f,0.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,0.0f,0.0f },
				{ 0.0f,0.0f,0.0f },
				{ 0.0f,0.0f,0.0f },
				{ 0.0f,0.0f,0.0f },
			}
		};
		ApplyStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(gfx, PSC2));
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		ApplyStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		ApplyStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
	};
	auto model = Sphere::Create_Advanced<Vertex>(latdist(rng), longdist(rng));
	// deform vertices of model by linear transformation
	model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.2f));

	ApplyBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	ApplyIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	ApplyBind(std::make_unique<TransformConstBuffer>(gfx, *this));
}

void LongLatSphere::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX LongLatSphere::FetchTransformMat() const noexcept
{
    return
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
