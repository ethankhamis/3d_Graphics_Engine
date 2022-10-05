#include "Camera.h"
#include "imgui/imgui.h"
DirectX::XMMATRIX Camera::FetchMatrix() const noexcept
{
	DirectX::XMVECTOR position;

	position = DirectX::XMVector3Transform
	(
		DirectX::XMVectorSet(0.f, 0.f, -radius, 0.f),
		DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	);

	DirectX::XMMATRIX matrix =
		DirectX::XMMatrixLookAtLH
		(
			position,
			DirectX::XMVectorZero(),
			DirectX::XMVectorSet(0.f,1.f, 0.f, 0.f)
		)
						*

		DirectX::XMMatrixRotationRollPitchYaw
		(
			pitch,-yaw,roll 
		);
	
	return matrix;
}

void Camera::ConstructControlWindow() noexcept
{
	if (ImGui::Begin("Camera View"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("Radius", &radius, 0.1f, 100.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);

		if (ImGui::Button("Reset")) { Resetv(); }
	}

	ImGui::End();
}

void Camera::Resetv() noexcept
{
	radius = 20.f;
	theta = .0f;
	phi = .0f;
	
	pitch = .0f;
	yaw = .0f;
	roll = .0f;
}
