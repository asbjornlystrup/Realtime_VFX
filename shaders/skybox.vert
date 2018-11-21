#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projMat;
uniform mat4 viewMat;

out vec3 vTextureDir;

void main() {
    gl_Position = projMat * viewMat * vec4(position, 1.0);
	vTextureDir = normalize(position);
}