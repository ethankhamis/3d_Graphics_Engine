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
	// creat vertex shouter output type
	VSOut vsoutput;
	//set position to multiplication of the position in NDC space
	vsoutput.position = mul(float4(position, 1.0f), transform);
	// set texture to the current texture coordinates
	vsoutput.texture_ = texture_;
	return vsoutput;
}