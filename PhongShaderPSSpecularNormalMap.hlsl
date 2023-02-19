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
cbuffer ObjectConstBuffer
{
    bool normal_map_state;
    float padding[3];
};

Texture2D tex;
Texture2D spec;
Texture2D normal_map;
SamplerState splr;


float4 main(float3 view_position : Position, float3 n : Normal,float3 tangent : Tangent,float3 bitangent : Bitangent, float2 tc : Texcoord) : SV_Target
{
    if (normal_map_state)
    {
        float3x3 tangent_to_world_space =
            float3x3(
            normalize(tangent), normalize(bitangent), normalize(n)
                );
        const float3 normal_sample = normal_map.Sample(splr, tc).xyz;
        n.x = normal_sample.x * 2.0f - 1.f;
        n.y = -normal_sample.y * 2.0f + 1.f;
        n.z = normal_sample.z;
        n.z = normal_sample.z;
        // translate normal to view space from tangent space
        n = mul(n, tangent_to_world_space);
    }
    // fragment to light vector data
    const float3 Vector_To_Light = lightPos - view_position;
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
    const float3 specular = attenuation * (diffuseColour * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(view_position))), specularPower);
    // final Colour in NRGB space by clamping using saturate
    // multiply diffuser lighting and ambient lighting by the texture colour alongside the  specular intensity and its colour
    // w = 1.f
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular * specularReflectionColour), 1.0f);
}