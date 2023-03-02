cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuse_colour;
    float diffuse_intensity;
    float constant_attentuation;
    float linear_attenuation;
    float quadratic_attenuation;
};