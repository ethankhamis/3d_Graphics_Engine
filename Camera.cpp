#include "Camera.h"
#include "imgui/imgui.h"
Camera::Camera() noexcept
{
	Resetv();
}
DirectX::XMMATRIX Camera::FetchMatrix() const noexcept
{
	using namespace DirectX;
	const vector FrontBase = DirectX::XMVectorSet(0.f, 0.f,1.0, 0.f);
	//application of camera rotations to frontbase vec
	const vector View = DirectX::XMVector3Transform
	(
		FrontBase,
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.f)
	);



	//create vector type of position
	const vector pos = DirectX::XMLoadFloat3(&position);
	// create target/direction using the view and position
	const vector target = pos + View;
	//create a matrix with the left handed coordinate system factoring:
	return DirectX::XMMatrixLookAtLH
	(
		pos, // camera position
		target, // target/eye position
		DirectX::XMVectorSet
		(
			0.f, 1.f, .0f, .0f // upward direction
		)
	);
}

void Camera::ConstructControlWindow() noexcept
{
	if (ImGui::Begin("Camera View"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &position.x, -90.f, 90.f, "%.1f");
		ImGui::SliderFloat("Y", &position.y, -90.f, 90.f, "%.1f");
		ImGui::SliderFloat("Z", &position.z, -90.f, 90.f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Pitch", &pitch,0.995f * -90.f, 0.995f * 90.f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);

		if (ImGui::Button("Reset")) { Resetv(); }
	}

	ImGui::End();
}

void Camera::Resetv() noexcept
{
	position = {0.2f, 3.7f, -21.2f };
	
	pitch = .0f;
	yaw = .0f;

}

void Camera::Rotate_by(float delta_x, float delta_y) noexcept
{
	//ensure yaw has respect to the 360 degree limit
	yaw = wrap_Radians(yaw + delta_x * rotation_speed);
	//clamp pitch between -90 degrees and 90 degrees (-1/2 pi and +1/2 pi)
	pitch = std::clamp(pitch + delta_y * rotation_speed, static_cast<float>(0.995f * -PI_LONG / 2.0f), static_cast<float>(0.995f * PI_LONG / 2.0f));
}

void Camera::Translate_by(float3 translation) noexcept
{
	//fill translation vector with the new translations (i.e., with respect to the rotation and travelspeed)
	DirectX::XMStoreFloat3
	(
		&translation, DirectX::XMVector4Transform
		(
			DirectX::XMLoadFloat3(&translation) //translation vector
			,
			DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.f)
			* // current perspective matrix scaled by travel speed  
			DirectX::XMMatrixScaling(travel_speed, travel_speed, travel_speed)
		)
	);

	//return new positions (updated)
	position = { position.x + translation.x,position.y + translation.y,position.z + translation.z };
}
