cbuffer ConstBuffer
{
	matrix modelView;
	matrix modelViewProj;
};

struct VertexShaderOutput
{
	float3 view_position : Position;
	float3 normal : Normal;
	float2 texture_coordinate : Texcoord;
	float4 pos : SV_Position;
};

VertexShaderOutput main(float3 pos : Position, float3 n : Normal, float2 texture_coordinates : Texcoord)
{
	//create VSOUT type
	VertexShaderOutput v_shaderoutput;
	//set world position to be the position multiplied by the view (model view)
	v_shaderoutput.view_position = (float3) mul(float4(pos, 1.0f), modelView);
	// set normal to the normal multiplied by the 3x3 modelview
	v_shaderoutput.normal = mul(n, (float3x3)modelView);
	// set the position to the position multiplied by the model projection / focal point
	v_shaderoutput.pos = mul(float4(pos, 1.0f), modelViewProj);
	v_shaderoutput.texture_coordinate = texture_coordinates;

	//return final VSOutput
	return v_shaderoutput;
}