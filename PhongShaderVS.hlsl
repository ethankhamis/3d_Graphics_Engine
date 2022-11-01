cbuffer CBuf
{
	matrix modelView;
	matrix modelViewProj;
};

struct VSOut
{
	float3 worldPos : Position;
	float3 normal : Normal;
	float2 texture_coord : Texcoord;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float2 texture_coord : Texcoord)
{
	VSOut v_shaderoutput;
	v_shaderoutput.worldPos = (float3) mul(float4(pos, 1.0f), modelView);
	v_shaderoutput.normal = mul(n, (float3x3)modelView);
	v_shaderoutput.pos = mul(float4(pos, 1.0f), modelViewProj);
	v_shaderoutput.texture_coord = texture_coord;
	return v_shaderoutput;
}