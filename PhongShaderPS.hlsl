cbuffer LightCBuf
{
	float3 lightPos;
};

static const float3 materialColour = { 0.9f,0.00f,0.00f };
static const float attConst = 1.0f;
static const float attLin = 0.05f;
static const float attQuad = 0.08f;
static const float3 ambient = { 0.09f,0.09f,0.09f };
static const float3 diffuseColour = { 1.0f,1.0f,1.0f };
static const float diffuseIntensity = 1.0f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	//fragment to light data for light
	const float3 vToL = lightPos - worldPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;

	// diffuse attenuation
	const float att = 1 / (attConst + attLin * distToL + attQuad * (distToL * distToL));

	// diffuse intensity

	const float3 diffuse = diffuseColour * diffuseIntensity * att * max(0.0f,dot(dirToL,n));
	
	// final colour

	return float4(saturate((diffuse + ambient) * materialColour),1.0f);
}