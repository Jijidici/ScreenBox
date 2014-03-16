#version 330

in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uTexture1;

out vec4 fragColor;

void main() {
	vec3 l = normalize(vec3(0., -1., -1.));
	float n_dot_l = dot(l, normalize(vNormal));

	vec3 color = n_dot_l*vec3(texture(uTexture1, vUV).rgb);
	
	fragColor = vec4(color, 1.);
}