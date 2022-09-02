Texture2D _texture;
SamplerState splr;

float4 main( float2 texture_coords : TexCoord) : SV_Target
{
	return tex.Sample(splr,texture_coords)
}