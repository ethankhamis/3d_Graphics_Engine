

cbuffer ConstantBuffer
{
	matrix modelView;
	matrix modelViewProjection;
};

struct VSOut
{
	float3 view_position : Position;
	float3 normal : Normal;
	float3 tangent : Tangent;
	float3 bitangent : Bitangent;
	float2 texture_coordinate : Texcoord;
	float4 position : SV_Position;
};

VSOut main(float3 position : Position, float3 normal : Normal, float3 tangent : Tangent, float3 bitangent : Bitangent, float2 texture_coordinate : Texcoord)
{
	VSOut vshader_output;
	vshader_output.view_position = (float3) mul(float4(position, 1.0f), modelView);
	vshader_output.normal = mul(normal, (float3x3)modelView);
	vshader_output.tangent = mul(tangent, (float3x3)modelView);
	vshader_output.bitangent = mul(bitangent, (float3x3)modelView);
	vshader_output.position = mul(float4(position, 1.f), modelViewProjection);
	vshader_output.texture_coordinate = texture_coordinate;
	return vshader_output;
}