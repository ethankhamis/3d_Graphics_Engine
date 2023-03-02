float3 apply_normal_map(const in float3 tangent,
	const in float3 bitangent,
	const in float3 n,
	const in float2 texcoord,
	uniform Texture2D normal_map,
	uniform SamplerState sampler_
)
{
	float3x3 tangent_to_view_space =
		float3x3(
			tangent, bitangent, n
			);
	const float3 normal_sample = normal_map.Sample(sampler_, texcoord).xyz;
	const float3 tanspace_n = normal_sample * 2.f - 1.f;
	return normalize(mul(tanspace_n, tangent_to_view_space));
}

float apply_attenuation(
	uniform float constant_attentuation, uniform float linear_attenuation, uniform float  quadratic_attenuation, const in float Distance_Vector_To__Light)
{
	return 1.0f / (constant_attentuation + linear_attenuation * Distance_Vector_To__Light + quadratic_attenuation * (Distance_Vector_To__Light * Distance_Vector_To__Light));
}

float3 apply_diffuse
(uniform float3 diffuse_colour,
	uniform float diffuse_intensity,
	const in float attenuation,
	const in float3 direction_vector_to_light,
	const in float3 n
)
{
	return diffuse_colour * diffuse_intensity * attenuation * max(0.0f, dot(direction_vector_to_light, n));
}

float3 apply_specular
(const in float3 specular_colour,
	uniform float specular_intensity,
	const in float3 n,
	const in float3 Direction_Vector_to_Light,
	const in float3 view_pos,
	const in float attenuation,
	const in float specular_power)
{
	//get vector from camera to light fragment in view space
	const float3 view_camera_to_light = normalize(view_pos);

	// calculate reflected light vector
	//perform the dot product on the vector to the light and the normal
	const float3 w = n * dot(Direction_Vector_to_Light, n);
	// calculate the reflected light vector
	const float3 r = w * 2.0f - Direction_Vector_to_Light;

	return attenuation * (specular_colour * specular_intensity) * pow(max(0.0f, dot(normalize(-r), normalize(view_camera_to_light))), specular_power);

}