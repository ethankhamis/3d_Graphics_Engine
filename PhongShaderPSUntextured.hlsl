cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColour;
    float diffuseIntensity;
    float attConst;
    float linear_attenuation;
    float Quadratic_attenuation;
};

cbuffer ObjectConstBuffer
{
    float3 materialColour;
    float specularIntensity;
    float specularPower;
    float padding[2];
};



float4 main(float3 view_position : Position, float3 n : Normal) : SV_Target
{
    // fragment to light vector data
    const float3 Vector_To_Light = lightPos - view_position;
    const float Distance_ofVector_To_Light = length(Vector_To_Light);
    const float3 Direction_Vector_To_Light = Vector_To_Light / Distance_ofVector_To_Light;
    // attenuation
    const float attenuation = 1.0f / (attConst + linear_attenuation * Distance_ofVector_To_Light + Quadratic_attenuation * (Distance_ofVector_To_Light * Distance_ofVector_To_Light));
    // diffuse intensity
    const float3 diffuse = diffuseColour * diffuseIntensity * attenuation * max(0.0f, dot(Direction_Vector_To_Light, n));
    // reflected light vector
    const float3 w = n * dot(Vector_To_Light, n);
    const float3 r = w * 2.0f - Vector_To_Light;
    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = attenuation * (diffuseColour * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(view_position))), specularPower);
    // final Colour
    // does not account for specular highlight
    return float4(saturate((diffuse + ambient) * materialColour +specular), 1.0f);
}