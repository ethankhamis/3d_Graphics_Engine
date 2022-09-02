struct VSOut
{
	float2 texture_ : TexCoord;
	float4 position : SV_Position;
};

cbuffer CBuf
{
	matrix transform;
};


VSOut main(float3 position : Position, float2 texture_ : TexCoord)
{
	VSOut vsoutput;
	vsoutput.position = mul(float4(position, 1.0f), transform);
	vsoutput.texture_ = texture_;
	return vsoutput;
}