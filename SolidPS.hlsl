cbuffer CBuf : register(b1)
{
	float4 colour;
};

float4 main() : SV_Target
{
	return colour;
}