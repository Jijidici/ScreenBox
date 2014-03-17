#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;
in vec3 vTangent;
in vec3 vBitangent;

uniform sampler2D uDiffuse;
uniform sampler2D uSpec;
uniform sampler2D uNormalMap;

out vec4 fragColor;
out vec4 fragNormal;

void main() {
	vec3 diff = texture(uDiffuse, vUV).rgb;
	float spec = texture(uSpec, vUV).r;
	vec3 bumpedNormal = normalize(texture(uNormalMap, vec2(vUV.x,vUV.y) ).rgb*2. - 1.);
	mat3 TBN = mat3(vTangent, vBitangent, vNormal);
	vec3 n = normalize(TBN*bumpedNormal);
	
	fragColor = vec4(diff, spec);
	fragNormal = vec4(n, 1.);
}