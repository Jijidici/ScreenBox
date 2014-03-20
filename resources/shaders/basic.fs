#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uDepth;
uniform sampler2D uFinal; 

out vec4 fragColor;

void main() {
	ivec2 fragCoord = ivec2(gl_FragCoord.xy);
	
	float depth = texelFetch(uDepth, fragCoord, 0).r;
	float realDepth = (2. * 0.1 * 100. / (100. + 0.1 - (2.*depth-1.) * (100. - 0.1)))/(100.-0.1);
	
	vec3 color = vec3(0.9f, 0.6f, 0.1f);
	if(vUV.x > 0.01 && vUV.x < 1.-0.01 && vUV.y > 0.01 && vUV.y < 1.-0.01) {
		vec3 invertedColor = 1. - texelFetch(uFinal, fragCoord, 0).rgb;
		color = mix(invertedColor, vec3(0.f), realDepth);
	}
	
	fragColor = vec4(color, 1.);
}