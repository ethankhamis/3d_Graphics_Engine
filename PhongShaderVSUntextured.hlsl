cbuffer CBuf
{
	matrix modelView;
	matrix modelViewProj;
};

struct VSOut
{
	float3 view_position : Position;
	float3 normal : Normal;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal)
{
	//create VSOUT type
	VSOut v_shaderoutput;
	//set world position to be the position multiplied by the view (model view)
	v_shaderoutput.view_position = (float3) mul(float4(pos, 1.0f), modelView);
	// set normal to the normal multiplied by the 3x3 modelview
	v_shaderoutput.normal = mul(n, (float3x3)modelView);
	// set the position to the position multiplied by the model projection / focal point
	v_shaderoutput.pos = mul(float4(pos, 1.0f), modelViewProj);
	return v_shaderoutput;
}