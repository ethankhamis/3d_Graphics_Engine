cbuffer CBuf
{
	matrix modelView;
	matrix modelViewProj;
};

struct VSOut
{
	float3 world_position : Position;
	float3 normal : Normal;
	float3 colour : Colour;
	float4 position : SV_Position;
};

VSOut main(float3 position : Position, float3 normal : Normal, float3 colour : Colour)
{
	VSOut vso;

	vso.world_position = (float3)mul(float4(position, 1.0f), modelView);
	vso.normal   = mul(normal, (float3x3)modelView);
	vso.position = mul(float4(position, 1.0f), modelViewProj);
	vso.colour = colour;
	return vso;
}