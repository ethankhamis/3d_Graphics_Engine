struct VSOut
{
	float4 colour : Color;
	float4 pos : SV_Position;
};

cbuffer CBuf
{
	matrix transform;
};

VSOut main(float3 pos : Position, float4 colour : Color)
{
	VSOut vsOut;
	vsOut.pos = mul(float4(pos, 1.0f), transform);
	vsOut.colour = colour;
	return vsOut;
}
