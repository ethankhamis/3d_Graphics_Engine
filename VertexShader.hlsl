float4 main(float2 pos : Position) : SV_Position //system value position
{
	return float4(pos.x,pos.y,0.0f,1.0f); //expand from 2d to 4d
}