cbuffer CBuf
{
	float4 colour;
};

float4 main() : SV_Target
{
	return colour;
}