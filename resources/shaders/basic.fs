#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;

out vec4 fragColor;

void main() {
	vec3 color = vec3(1.f, 0.f, 1.f);
	
	fragColor = vec4(color, 1.);
}