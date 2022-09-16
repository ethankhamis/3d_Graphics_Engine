#include "PhongShadingCube.h"
#include "DefaultBindables.h"
#include "Cube.h"
#include "ThrowMacros.h"
#include "imgui/imgui.h"

PhongShadingCube::PhongShadingCube(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 material)
	:
	TestObject( gfx, rng, adist, ddist, odist, rdist )
{
	if (!is_static_init())
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
		};
		auto model = Cube::Create_Independentf<Vertex>();
		model.ApplyNormalsIndependent();

		ApplyStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongShaderVS.cso");
		auto pvsbc = pvs->FetchByteCode();
		ApplyStaticBind(std::move(pvs));

		ApplyStaticBind(std::make_unique<PixelShader>(gfx, L"PhongShaderPS.cso"));

		ApplyStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		ApplyStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		ApplyStaticBind(std::make_unique<PrimTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		ApplyIndexFromStatic();
	}

	ApplyBind(std::make_unique<TransformConstBuffer>(gfx, *this));

	material_constants.colour = material;
	ApplyBind(std::make_unique<PixelConstantBuffer<MaterialConstPS>>(gfx, material_constants, 1u));

	// model deformation transform (per instance, not stored as bind)
	DirectX::XMStoreFloat3x3(
		&model_transform,
		DirectX::XMMatrixScaling(2.0f, 2.0f, 2.3f)
	);
}


DirectX::XMMATRIX PhongShadingCube::FetchTransformMat() const noexcept
{
	return
		DirectX::XMLoadFloat3x3(&model_transform) *
		TestObject::FetchTransformMat();
}

bool PhongShadingCube::ControlWnd(int identifier, Graphics& gfx) noexcept
{
	bool old = NULL;
	bool isOpen = true;

	if (ImGui::Begin(("Box " + std::to_string(identifier)).c_str(), &isOpen))
	{
		ImGui::TextColored(ImVec4{ 255,0,0,1 }, "Material Properties");
		const bool matColour = ImGui::ColorEdit3("Material Colour", &material_constants.colour.x);
		const bool specIntensity = ImGui::SliderFloat("Specular Intensity", &material_constants.specular_intensity, .05f, 4.f, "%.2f", 2);
		const bool specPow = ImGui::SliderFloat("Specular Power", &material_constants.specularPow, 1.f, 200.f, "%.2f", 2);
		old =
			matColour ||
			specIntensity ||
			specPow;


		ImGui::Text("Object Position");
		ImGui::SliderFloat("Radius", &r, 0.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta, -180.f, 180.f);
		ImGui::SliderAngle("Phi", &phi, -180.f, 180.f);
		ImGui::Text("Object Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
	}

	ImGui::End();

	if (old)
	{
		RefreshMaterial(gfx);
	}return isOpen;
}

void PhongShadingCube::RefreshMaterial(Graphics& gfx) noexcept(!Debug)
{
	assert(QueryBindableObj<PixelConstantBuffer<MaterialConstPS>>() != nullptr);
	QueryBindableObj<PixelConstantBuffer<MaterialConstPS>>()->Update(gfx, material_constants);
}
