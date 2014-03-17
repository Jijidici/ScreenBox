#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;
in vec3 vBitangent;

uniform vec3 uCameraPosition;
uniform sampler2D uDiffuse;
uniform sampler2D uSpec;
uniform sampler2D uNormalMap;

out vec4 fragColor;

void main() {
	vec3 lightPosition = vec3(-0.5, 2.f, -1.f);
	vec3 lightColor = vec3(0.8, 1., 0.8);
	float lightIntensity = 2.f;

	vec3 diff = texture(uDiffuse, vUV).rgb;
	float spec = texture(uSpec, vUV).r;
	vec3 bumpedNormal = normalize(texture(uNormalMap, vec2(vUV.x,vUV.y) ).rgb*2. - 1.);
	mat3 TBN = mat3(vTangent, vBitangent, vNormal);
	
	vec3 n = normalize(TBN*bumpedNormal);
	vec3 l = lightPosition - vPosition;
	vec3 v = vPosition - uCameraPosition;
	vec3 h = normalize(l-v);
	
	float n_dot_l = clamp(dot(n, l), 0, 1.);
	float n_dot_h = clamp(dot(n, h), 0, 1.);

	float d = length(l);
	float att = 1./(d*d);

	vec3 color = lightColor * lightIntensity * att * (diff*n_dot_l + spec*vec3(1., 1., 1.)*pow(n_dot_h, spec*100.));
	
	fragColor = vec4(color, 1.);
}