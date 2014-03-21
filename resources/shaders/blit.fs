#version 330

in vec2 vUV;

uniform sampler2D uTexture1;
uniform float uGamma;

out vec4 fragColor;

void main() {
	vec3 color = texture(uTexture1, vUV).rgb;
	
	if(uGamma > 0.) {
		color.r = pow(color.r, 1./uGamma);
		color.g = pow(color.g, 1./uGamma);
		color.b = pow(color.b, 1./uGamma);
	}
	
	fragColor = vec4(color, 1.);
}