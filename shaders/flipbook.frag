#version 330 core

struct DirectionalLight {
	int enabled;
	vec3 color;
	vec3 direction;
	vec3 ambientColor;
};

struct PointLight {
	int enabled;
	vec3 color;
	vec3 position;
	float radius;
	vec3 ambientColor;
};

in vec2 vTexPos;
in vec3 vWorldPos;
in vec3 vWorldNormal;
in mat3 vTBNMat;

uniform sampler2D texNormalMapCurr;
uniform sampler2D texDiffuseAlphaMapCurr;
uniform sampler2D texOpticalFlowMapCurr;
uniform sampler2D texNormalMapNext;
uniform sampler2D texDiffuseAlphaMapNext;
uniform sampler2D texOpticalFlowMapNext;
uniform samplerCube texEnvMap;
uniform vec3 camPos;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform vec3 kDiffuse;
uniform vec3 kSpecular;
uniform float specularExponent;
uniform float blendFactor;
uniform vec2 distortionStrength;

out vec4 color;

vec4 depremultiply(vec4 color) {
	if (color.a != 0.0) {
		color.rgb /= color.a;
	}

	return color;
}

void main() {
	vec2 velocityCurr = texture(texOpticalFlowMapCurr, vTexPos).xy * 2.0 - 1.0;
	vec2 velocityNext = texture(texOpticalFlowMapNext, vTexPos).xy * 2.0 - 1.0;
	
	vec2 texPosCurr = vTexPos - velocityCurr * blendFactor * distortionStrength;
	vec2 texPosNext = vTexPos + velocityNext * (1.0 - blendFactor) * distortionStrength;

	vec3 normalMapSample = normalize(mix(normalize(texture(texNormalMapCurr, texPosCurr).rgb * 2.0 - 1.0), 
			normalize(texture(texNormalMapNext, texPosNext).rgb * 2.0 - 1.0), blendFactor));
	vec3 worldNormal = vTBNMat * normalMapSample;

	vec4 diffuseAlphaMapSample = depremultiply(mix(texture(texDiffuseAlphaMapCurr, texPosCurr), 
			texture(texDiffuseAlphaMapNext, texPosNext), blendFactor));

	color = vec4(0, 0, 0, diffuseAlphaMapSample.a);

	// Environment mapping

	vec3 camRay = normalize(vWorldPos - camPos);
	vec3 camRayReflected = camRay - 2.0 * dot(camRay, worldNormal) * worldNormal;
	vec3 envMapSample = texture(texEnvMap, camRayReflected).rgb;
	color.rgb += envMapSample * kSpecular;

	// Phong lighting

	if (directionalLight.enabled == 1) {
		vec3 toLight = normalize(-directionalLight.direction);

		float diffuseDotProd = dot(toLight, worldNormal);
		float diffuseFactor = max(0.0, diffuseDotProd);
	
		vec3 toLightReflected = 2.0 * diffuseDotProd * worldNormal - toLight;
		vec3 toCam = normalize(camPos - vWorldPos);
		float specularFactor = pow(max(0.0, dot(toLightReflected, toCam)), specularExponent);

		color.rgb += directionalLight.color * (
				diffuseAlphaMapSample.rgb * kDiffuse * (diffuseFactor + directionalLight.ambientColor) +
				kSpecular * specularFactor);
	}

	if (pointLight.enabled == 1) {
		vec3 toLight = normalize(pointLight.position - vWorldPos);

		float diffuseDotProd = dot(toLight, worldNormal);
		float diffuseFactor = max(0.0, diffuseDotProd);
	
		vec3 toLightReflected = 2.0 * diffuseDotProd * worldNormal - toLight;
		vec3 toCam = normalize(camPos - vWorldPos);
		float specularFactor = pow(max(0.0, dot(toLightReflected, toCam)), specularExponent);

		float attenuation = distance(pointLight.position, vWorldPos) * 0.3;
		color.rgb += pointLight.color * (
				diffuseAlphaMapSample.rgb * kDiffuse * (diffuseFactor + directionalLight.ambientColor) +
				kSpecular * specularFactor) / attenuation;
	}
}