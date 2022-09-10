
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
	float specularIntensity;
	float specularPower;
	float padding[2];
};


float4 main(float3 world_position : Position, float3 n : Normal, float3 colour : Colour) : SV_Target
{
	//light vector data

	const float3 vToL = light_position - world_position;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;

	//attenuation calculation
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	
	// diffuse intensity calculation
	const float3 diffuse = diffuseColour * diffuseIntensity * att * max(0.0f,dot(dirToL,n));
	
	// calculate reflected light vector
	const float3 w = n * dot(vToL,n);
	const float3 r = w * 2.0f - vToL;

	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
	const float3 specular = att * (diffuseColour * diffuseIntensity) * specularIntensity * pow(max(0.0f,dot(normalize(-r),normalize(world_position))),specularPower);
	
	// final Colour
	return float4(saturate((diffuse + ambient + specular) * colour), 1.0f);
}