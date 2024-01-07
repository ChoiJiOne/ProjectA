#version 460 core

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inWorldPositionInLightSpace;

layout(location = 0) out vec4 outColor;

struct Material
{
	vec3 ambientRGB;
	vec3 diffuseRGB;
	vec3 specularRGB;
	float shininess;
};

struct DirectionalLight
{
	vec3 position;
	vec3 direction;

	vec3 ambientRGB;
	vec3 diffuseRGB;
	vec3 specularRGB;
};

float ShadowCalculation(vec4 worldPositionInLightSpace);
vec3 ComputeDirectionalLight(in DirectionalLight light, in Material material, in vec3 normal, in vec3 viewDirection);

layout(binding = 0) uniform sampler2D shadowMap;
uniform vec3 viewPosition;
uniform DirectionalLight directionalLight;
uniform Material material;

void main()
{
	vec3 norm = normalize(inNormal);
	vec3 viewDirection = normalize(viewPosition - inWorldPosition);

	vec3 outputRGB = ComputeDirectionalLight(directionalLight, material, norm, viewDirection);
	
	outColor = vec4(outputRGB, 1.0f);
}

float ShadowCalculation(vec4 worldPositionInLightSpace)
{
	vec3 projectCoords = worldPositionInLightSpace.xyz / worldPositionInLightSpace.w;
	projectCoords = projectCoords * 0.5f + 0.5f;

	float closestDepth = texture(shadowMap, projectCoords.xy).r;
	float currentDepth = projectCoords.z;

	return currentDepth > closestDepth  ? 1.0f : 0.0f;
}

vec3 ComputeDirectionalLight(in DirectionalLight light, in Material material, in vec3 normal, in vec3 viewDirection)
{
	// ambient
	vec3 ambientRGB = light.ambientRGB * material.ambientRGB;

	// diffuse
	vec3 lightDirection = normalize(-light.direction);
	float diff = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuseRGB = light.diffuseRGB * diff * material.diffuseRGB;

	// specular
	vec3 halfDirection = normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfDirection), 0.0f), material.shininess);
	vec3 specularRGB = light.specularRGB * spec * material.specularRGB;
	
	// Shadow
	float shadow = ShadowCalculation(inWorldPositionInLightSpace);   

	return (ambientRGB + (1.0f - shadow) * (diffuseRGB + specularRGB));
}
