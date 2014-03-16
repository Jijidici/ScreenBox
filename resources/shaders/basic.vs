#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

out vec3 vNormal;
out vec2 vUV;

void main() {
	vNormal = normal;
	vUV = texcoords;
	gl_Position = vec4(position, 1.f);
}