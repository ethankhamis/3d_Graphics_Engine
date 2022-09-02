Texture2D texture_;

SamplerState sampler_s;

float4 main(float2 texCoords : TexCoord) : SV_Target
{
	return texture_.Sample(sampler_s,texCoords);
}