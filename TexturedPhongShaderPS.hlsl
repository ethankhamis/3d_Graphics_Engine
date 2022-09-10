cbuffer LightCBuf
{
float3 light_position;
float3 ambient;
float3 diffuseColour;
float diffuseIntensity;
float attConst;
float attLin;
float attQuad;
};

cbuffer ObjectCBuf
{
	float specular_intensity;
	float specularPow;
	float padding[2];
};

Texture2D tex;

SamplerState splr;


float4 main(float3 world_position : Position, float3 normal : Normal, float2 tc : Texcoord) : SV_Target
{
	// fragment to light vector data
	const float3 vToL = light_position - world_position;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;
	// attenuation
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
	const float3 diffuse = diffuseColour * diffuseIntensity * att * max(0.0f,dot(dirToL, normal));
	// reflected light vector
	const float3 w = normal * dot(vToL, normal);
	const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
	const float3 specular = att * (diffuseColour * diffuseIntensity) * specular_intensity * pow(max(0.0f,dot(normalize(-r),normalize(world_position))),specularPow);
	// final colour
	return float4(saturate(diffuse + ambient + specular), 1.0f) * tex.Sample(splr, tc);
}