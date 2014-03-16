#version 330

in vec3 vNormal;
in vec2 vUV;

out vec4 fragColor;

void main() {
	fragColor = vec4(vUV, vNormal.z, 1.f);
}