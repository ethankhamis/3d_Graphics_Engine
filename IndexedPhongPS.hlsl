cbuffer LightCBuf
{
	float3 lightPos;
	float3 ambient;
	float3 diffuseColour;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

cbuffer ObjectCBuf
{
	float3 materialColours[6];
	float padding;
	float specularIntensity;
	float specularPower;
};


float4 main(float3 worldPos : Position, float3 n : Normal, uint tid : SV_PrimitiveID) : SV_Target
{
	// fragment to light vector data
	const float3 vToL = lightPos - worldPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;
	// attenuation
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
	const float3 diffuse = diffuseColour * diffuseIntensity * att * max(0.0f,dot(dirToL,n));
	// reflected light vector
	const float3 w = n * dot(vToL,n);
	const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
	const float3 specular = att * (diffuseColour * diffuseIntensity) * specularIntensity * pow(max(0.0f,dot(normalize(-r),normalize(worldPos))),specularPower);
	// final color
	return float4(saturate((diffuse + ambient + specular) * materialColours[tid % 6]),1.0f);
}