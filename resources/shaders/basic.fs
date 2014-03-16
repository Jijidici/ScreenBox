#version 330

in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uTexture1;

out vec4 fragColor;

void main() {
	fragColor = texture(uTexture1, vUV);
}