#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 worldMat;
uniform vec3 camPos;
uniform vec3 camUp;

out vec2 vTexPos;
out vec3 vWorldPos;
out vec3 vWorldNormal;
out mat3 vTBNMat;

void main() {
	vec3 centerWorldPos = (worldMat * vec4(0, 0, 0, 1)).xyz;

	vWorldNormal = normalize(camPos - centerWorldPos);
	vec3 tangent = normalize(cross(camUp, vWorldNormal));
	vec3 biTangent = cross(vWorldNormal, tangent);
	vTBNMat = mat3(tangent, biTangent, vWorldNormal);

	vWorldPos = centerWorldPos + vTBNMat * mat3(worldMat) * position;

	gl_Position = projMat * viewMat * vec4(vWorldPos, 1.0);
	
	vTexPos = position.xy * vec2(0.5, -0.5) + 0.5;
}