#version 330 core

in vec3 vTextureDir;

uniform samplerCube tex;

out vec4 color;

void main() {
    color = texture(tex, vTextureDir);
}