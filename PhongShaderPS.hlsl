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
	float3 materialColour;
	float specularIntensity;
	float specularPower;
};

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	// fragment to light vector data
	const float3 vToL = lightPos - worldPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;
	// diffuse attenuation
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
	const float3 diffuse = diffuseColour * diffuseIntensity * att * max(0.0f,dot(dirToL,n));

	//vector of reflected light
	const float3 w = n * dot(dirToL, n);
	const float3 r = w * 2.f - dirToL;

	//specular highlights using angle betwen viewer vector and reflected vector -- narrowed
	
	const float3 specular_highlight = (diffuseColour * diffuseIntensity) * specularIntensity * pow(max(0.f, dot(normalize(r), normalize(worldPos))), specularPower);

	// final color
	return float4(saturate((diffuse + ambient + specular_highlight) * materialColour),1.0f);
}