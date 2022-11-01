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

Texture2D texture_;
Texture2D specular;
SamplerState sampler_state_;

float4 main(float3 worldPos : Position, float3 n : Normal, float2 texture_coord : Texcoord) : SV_Target
{
	//pixel to light vector data
	const float3 vToL = lightPos - worldPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;

	// attenuation calculation
	const float attenuation =
		1.0f /
		(attConst + attLin * distToL + attQuad * (distToL * distToL));

	// diffuse intensity calculation
	const float3 diffuse = diffuseColour * diffuseIntensity * attenuation * max(0.0f , dot(dirToL,n));

	//vector of reflected light
	const float3 w = n * dot(vToL, n);
	const float3 r = w * 2.f - vToL;

	//specular highlights using angle betwen viewer vector and reflected vector -- narrowed

	const float4 specSample = specular.Sample(sampler_state_, texture_coord);
	const float3 specularReflection = specSample.rgb;
	const float specularPower = pow(2.0f,specSample.a * 13.0f);
	const float3 specular_highlight = attenuation * (diffuseColour * diffuseIntensity) * pow(max(0.f, dot(normalize(-r), normalize(worldPos))), specularPower);

	// final color
	return float4(saturate((diffuse + ambient) * texture_.Sample(sampler_state_, texture_coord).rgb + specular_highlight * specularReflection),1.0f);
}