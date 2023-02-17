#pragma once
#include "Graphics.h"
#include "BindSolidLongLatSphere.h"
#include "ConstBuffer(s).h"
#include "debugdefs.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f); //default radius
	void ControlWnd() noexcept; // spawn control window
	void Reset() noexcept; // reset all attributes
	void Render(Graphics& gfx) const noexcept_unless; // remder light
	void Update(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
	struct PointLightCBuf
	{//using alignas to prevent buffer size errors
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient; //rgb colour of of ambient lighting
		alignas(16) DirectX::XMFLOAT3 diffuseColour; //rgb colour of diffuse light
		float diffuseIntensity;
		// attenuation attributes using quadratic
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf constbuffer_data;
	mutable SolidLongLatSphere mesh; // model of light source
	mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf; //constant buffer for pixel shader
};