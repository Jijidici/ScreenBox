#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uDepth;
uniform sampler2D uFinal; 

out vec4 fragColor;

vec3 getInvertedFilter() {
	ivec2 fragCoord = ivec2(gl_FragCoord.xy);
	
	float depth = texelFetch(uDepth, fragCoord, 0).r;
	float realDepth = (2. * 0.1 * 100. / (100. + 0.1 - (2.*depth-1.) * (100. - 0.1)))/(100.-0.1);
	
	vec3 invertedColor = 1. - texelFetch(uFinal, fragCoord, 0).rgb;
	vec3 retColor = mix(invertedColor, vec3(0.f), realDepth);
	
	return retColor;
}

vec3 getToonFilter() {
	vec3 retColor = vec3(1.f, 0.f, 1.f);
	
	// gradient matrix
	mat3 G[2] = mat3[](
        mat3( 1.0, 2.0, 1.0, 0.0, 0.0, 0.0, -1.0, -2.0, -1.0 ),
        mat3( 1.0, 0.0, -1.0, 2.0, 0.0, -2.0, 1.0, 0.0, -1.0 )
	);
	
	// quantification
	retColor = 5.*texelFetch( uFinal, ivec2(gl_FragCoord), 0).rgb;
	ivec3 integerColor = ivec3(retColor.r, retColor.g, retColor.b);
	retColor = vec3(integerColor.r*0.2, integerColor.g*0.2, integerColor.b*0.2);
	if(retColor.r > 0.) { retColor.r += 0.1; }
	if(retColor.g > 0.) { retColor.g += 0.1; }
	if(retColor.b > 0.) { retColor.b += 0.1; }
	
	// cell shading
	mat3 I;
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
				vec3 sample  = texelFetch(uFinal, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0).rgb;
				I[i][j] = length(sample); 
		}
	}

	float gx = dot(G[0][0], I[0]) + dot(G[0][1], I[1]) + dot(G[0][2], I[2]);
	float gy = dot(G[1][0], I[0]) +  dot(G[1][1], I[1]) + dot(G[1][0], I[2]);
	float g = sqrt(gx*gx + gy*gy);
	retColor = 2.*retColor - (vec3(g)*0.5);
	
	return retColor;
}

void main() {
	vec3 color =  vec3(0.9f, 0.6f, 0.1f);
	if(vUV.x > 0.01 && vUV.x < 1.-0.01 && vUV.y > 0.01 && vUV.y < 1.-0.01) {
		color = getToonFilter();
	}	
	
	fragColor = vec4(color, 1.);
}