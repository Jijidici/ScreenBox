#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;

uniform vec3 uCameraPosition;
uniform sampler2D uDiffuse;
uniform sampler2D uSpec;

out vec4 fragColor;

void main() {
	vec3 lightPosition = vec3(-0.5, 0.5, -1.);
	vec3 lightColor = vec3(0.8, 1., 0.8);
	float lightIntensity = 2.f;

	vec3 diff = texture(uDiffuse, vUV).rgb;
	float spec = texture(uSpec, vUV).r;
	
	vec3 n = normalize(vNormal);
	vec3 l = lightPosition - vPosition;
	vec3 v = vPosition - uCameraPosition;
	vec3 h = normalize(l-v);
	
	float n_dot_l = clamp(dot(n, l), 0, 1.);
	float n_dot_h = clamp(dot(n, h), 0, 1.);

	float d = distance(lightPosition, vPosition);
	float att = clamp(1./(1.+ 1.*(d*d)), 0., 1.);

	vec3 color = lightColor * lightIntensity * att * (diff*n_dot_l + spec*vec3(1., 1., 1.)*pow(n_dot_h, spec*100.));
	
	fragColor = vec4(color, 1.);
}