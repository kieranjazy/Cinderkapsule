#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 FragColour;

layout(location = 0) in vec2 TexCoords;
layout(location = 1) in vec3 WorldPos;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 camPos;
layout(location = 4) in vec3 inLightPos1;
layout(location = 5) in vec3 inLightPos2;
layout(location = 6) in vec3 inLightColour1;
layout(location = 7) in vec3 inLightColour2;


layout(binding = 1) uniform sampler2D albedoMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D metallicMap;
layout(binding = 4) uniform sampler2D roughnessMap;
layout(binding = 5) uniform sampler2D aoMap;


const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 getNormalFromNormalMap() {
	vec3 normal = texture(normalMap, TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);


	return normal;
}

void main() {
	vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2, 2.2, 2.2));
	vec3 normal = getNormalFromNormalMap();
	float metallic = texture(metallicMap, TexCoords).r;
	float roughness = texture(roughnessMap, TexCoords).r;
	float ao = texture(aoMap, TexCoords).r;

	vec3 lightPositions[] = {inLightPos1, inLightPos2};
	vec3 lightColours[] = {inLightColour1, inLightColour2};

	vec3 N = normalize(Normal);
	vec3 V = normalize(camPos - WorldPos);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);

	for(int i = 0; i != 2; ++i) {
		vec3 L = normalize(lightPositions[i] - WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(lightPositions[i] - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColours[i] * attenuation;

		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		vec3 num = NDF * G * F;
		float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		vec3 specular = num / denom;

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 colour = ambient + Lo;

	colour = colour / (colour + vec3(1.0));
	colour = pow(colour, vec3(1.0/2.2));

	FragColour = vec4(colour, 1.0);
	//FragColour = texture(albedoMap, TexCoords);
}