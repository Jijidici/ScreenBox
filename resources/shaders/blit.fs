#version 330

in vec2 vUV;

uniform sampler2D uTexture1;

out vec4 fragColor;

void main() {
	vec3 color = texture(uTexture1, vUV).rgb;
	
	fragColor = vec4(color, 1.);
}