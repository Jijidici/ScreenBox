#version 330

in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uTexture1;

out vec4 fragColor;

void main() {
	fragColor = vec4(vUV, 0., 1.);
	//fragColor = texture(uTexture1, vUV);
}