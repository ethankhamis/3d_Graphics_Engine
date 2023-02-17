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

Texture2D tex;
Texture2D spec;

SamplerState splr;


float4 main(float3 worldPos : Position, float3 n : Normal, float2 tc : Texcoord) : SV_Target
{
    // fragment to light vector data
    const float3 Vector_To_Light = lightPos - worldPos;
    const float Distance_Vector_To__Light = length(Vector_To_Light);
    const float3 Direction_Vector_To_Light = Vector_To_Light / Distance_Vector_To__Light;
    // attenuation calculation
    const float attenuation = 1.0f / (attConst + attLin * Distance_Vector_To__Light + attQuad * (Distance_Vector_To__Light * Distance_Vector_To__Light));
    // diffuse intensity calculation
    const float3 diffuse = diffuseColour * diffuseIntensity * attenuation * max(0.0f, dot(Direction_Vector_To_Light, n));
   
    // calculate reflected light vector
    //perform the dot product on the vector to the light and the normal
    const float3 w = n * dot(Vector_To_Light, n);
    // calculate the reflected light vector
    const float3 r = w * 2.0f - Vector_To_Light;


    // calculate specular intensity based on angle between the viewing vector and reflection vector

    const float4 specularSample = spec.Sample(splr, tc);
    const float3 specularReflectionColour = specularSample.rgb;
    //narrow it down using with power function
    const float specularPower = pow(2.0f, specularSample.a * 13.0f);
    //create a float string specular intensity, direction and its calculated power
    const float3 specular = attenuation * (diffuseColour * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
    // final Colour in NRGB space by clamping using saturate
    // multiply diffuser lighting and ambient lighting by the texture colour alongside the  specular intensity and its colour
    // w = 1.f
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular * specularReflectionColour), 1.0f);
}