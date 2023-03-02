#include "pointlight_data.hlsli"
#include "complex_operations.hlsli"
cbuffer ObjectConstBuffer
{
    bool normal_map_state;
    float padding[3];
};

Texture2D tex;
Texture2D spec;
Texture2D normal_map;
SamplerState sampler_;


float4 main(float3 view_position : Position, float3 n : Normal,float3 tangent : Tangent,float3 bitangent : Bitangent, float2 tc : Texcoord) : SV_Target
{
    if (normal_map_state)
    {
        n = apply_normal_map(normalize(tangent), normalize(bitangent), n, tc, normal_map, sampler_);
    }
    // fragment to light vector data
    const float3 Vector_To_Light = lightPos - view_position;
    const float Distance_Vector_To__Light = length(Vector_To_Light);
    const float3 Direction_Vector_To_Light = Vector_To_Light / Distance_Vector_To__Light;
    // attenuation calculation
    const float attenuation = apply_attenuation(constant_attentuation,linear_attenuation ,quadratic_attenuation , Distance_Vector_To__Light);
    // diffuse intensity calculation
    const float3 diffuse = apply_diffuse(diffuse_colour, diffuse_intensity, attenuation, Direction_Vector_To_Light, n);
 

    // calculate specular intensity based on angle between the viewing vector and reflection vector

    const float4 specularSample = spec.Sample(sampler_, tc);
    const float3 specularReflectionColour = specularSample.rgb;
    //narrow it down using with power function
    const float specularPower = pow(2.0f, specularSample.a * 13.0f);
    //create a float string specular intensity, direction and its calculated power
    const float3 specular = apply_specular(specularReflectionColour, 1.f, n, Direction_Vector_To_Light,
        view_position, attenuation, specularPower);
    // final Colour in NRGB space by clamping using saturate
    // multiply diffuser lighting and ambient lighting by the texture colour alongside the  specular intensity and its colour
    // w = 1.f
    return float4(saturate((diffuse + ambient) * tex.Sample(sampler_, tc).rgb + specular * specularReflectionColour), 1.0f);
}