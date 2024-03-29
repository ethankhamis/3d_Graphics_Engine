//slot 0
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
//slot 1
cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};
//slot 2
cbuffer TransformCBuf
{
    matrix modelView;
    matrix modelViewProjection;
};

Texture2D tex;
Texture2D nmap : register(t2);

SamplerState splr;




float4 main(float3 view_position : Position, float3 n : Normal, float3 tangent : Tangent, float3 bitangent : Bitangent, float2 tc : Texcoord) : SV_Target
{
    // sample normal from map if normal mapping enabled == true
    if (normalMapEnabled)
    {
      
        //translate normal map into view space
        const float3 normalSample = nmap.Sample(splr, tc).xyz;
        n.x = normalSample.x * 2.0f - 1.0f;
        n.y = -normalSample.y * 2.0f + 1.0f;
        n.z = -normalSample.z * 2.f + 1.f;
        n = normalize(mul(n, (float3x3)modelView));
 }
// fragment to light vector data
const float3 vToL = lightPos - view_position;
const float distToL = length(vToL);
const float3 dirToL = vToL / distToL;
// attenuation
const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
// diffuse intensity
const float3 diffuse = diffuseColour * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
// reflected light vector
const float3 w = n * dot(vToL, n);
const float3 r = w * 2.0f - vToL;
// calculate specular intensity based on angle between viewing vector and reflection vector,
// narrow with power function
const float3 specular = att * (diffuseColour * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(view_position))), specularPower);
// final Colour
return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}