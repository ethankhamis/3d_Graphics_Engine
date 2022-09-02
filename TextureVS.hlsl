cbuffer CBuf
{
	matrix transform
};

struct VSOut
{
	float2 texture_coords : TexCoord;
	float4 position : SV_Position;
};

VSOut main(float3 position : Position, float2 texture_coordinates : TexCoord)
{
	VSOut vso;
	vso.position = mul(float4(position, 1.0f), transform);
	vso.texture_coordinates = texture_coordinates;
	return vso;
}