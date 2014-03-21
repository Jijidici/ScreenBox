#version 330

in vec3 vPosition;
in vec3 vViewSpacePosition;
in vec3 vNormal;
in vec2 vUV;

uniform ivec2 uWinSize;
uniform sampler2D uMaterial;
uniform sampler2D uNormal;
uniform sampler2D uDepth;
uniform sampler2D uFinal;
uniform int uID;

out vec4 fragColor;

// UTILITAR FUNCTIONS
// Hue - Saturation - Luminance to Red - Green - Blue model conversion
vec3 HSLtoRGB(int h, float s, float l){
	vec3 color;	
	float c = (1. - abs(2 * l - 1.)) * s;
	float m = 1. * (l - 0.5 * c);
	float x = c * (1. - abs(mod(h / 60., 2) - 1.));

	h = h%360;
	if(h >= 0 && h < 60){ color = vec3(c + m, x + m, m); }
	else if(h >= 60 && h < 120){ color = vec3(x + m, c + m, m);	}
	else if(h >= 120 && h < 180){ color = vec3(m, c + m, x + m); }
	else if(h >= 180 && h < 240){ color = vec3(m, x + m, c + m); }
	else if(h >= 240 && h < 300){ color = vec3(x + m, m, c + m); }
	else if(h >= 300 && h < 360){ color = vec3(c + m, m, x + m); }
	else{ color = vec3(m, m, m); }
	return color;
}

vec3 RGBtoHSL(float r, float g, float b){
	float h = 0;
	float s = 0;
	float l = 0;

	float var_min = min(min(r, g), b);
	float var_max = max(max(r, g), b);
	float del_max = var_max - var_min;

	l = (var_max + var_min)*0.5;

	if(del_max != 0){
		if(l < 0.5) s = del_max / (var_max + var_min);
		else 		s = del_max / (2 - var_max - var_min);

		float del_r = (((var_max - r)/6.) + (del_max*0.5)) / del_max;
		float del_g = (((var_max - g)/6.) + (del_max*0.5)) / del_max;
		float del_b = (((var_max - b)/6.) + (del_max*0.5)) / del_max;

		if(r == var_max) h = del_b - del_g;
		else if(g == var_max) h = 1./3. + del_r - del_b;
		else if(b == var_max) h = 2./3. + del_g - del_r;

		if(h < 0) h += 1;
		if(h > 1) h -= 1;
		h *= 360;
		return vec3(h,s,l);
	}
}

float getRealDepth(float depth) {
	return 20./(100.1 - (2.*depth-1.) * 99.9);
}

float getRealDepthFromQuad(float depth) {
	float quadFragDepth = length(vViewSpacePosition);
	return getRealDepth(depth) - quadFragDepth;
}

// SHADING FUNCTIONS
vec3 getInvertedFilter() {
	ivec2 fragCoord = ivec2(gl_FragCoord.xy);
	
	float depth = texelFetch(uDepth, fragCoord, 0).r;
	float realDepth = (2. * 0.1 * 100. / (100. + 0.1 - (2.*depth-1.) * (100. - 0.1)))/(100.-0.1);
	
	vec3 invertedColor = 1. - texelFetch(uFinal, fragCoord, 0).rgb;
	vec3 retColor = mix(invertedColor, vec3(0.f), realDepth);
	
	return retColor;
}

vec3 getToonFilter() {
	vec3 retColor = texelFetch( uFinal, ivec2(gl_FragCoord), 0).rgb;
	
	// gradient matrix
	mat3 G[2] = mat3[](
        mat3( 1.0, 2.0, 1.0, 0.0, 0.0, 0.0, -1.0, -2.0, -1.0 ),
        mat3( 1.0, 0.0, -1.0, 2.0, 0.0, -2.0, 1.0, 0.0, -1.0 )
	);
	
	// quantification
	retColor = 5.*retColor;
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

vec3 getSaturatedFilter() {
	ivec2 fragCoord = ivec2(gl_FragCoord);

	vec3 retColor = texelFetch( uFinal, fragCoord, 0).rgb;
	vec3 hslColor = RGBtoHSL(retColor.r, retColor.g, retColor.b);
	
	float depth = texelFetch(uDepth, fragCoord, 0).r;
	float realDepth = getRealDepthFromQuad(depth);
	
	hslColor.y = 1. - max(min((realDepth)-1.5, 1.), 0.);
	retColor = HSLtoRGB(int(hslColor.x-90.), hslColor.y, hslColor.z);
	
	return retColor;
}

vec3 getXRayFilter() {
	vec3 retColor = vec3(1., 0., 1.);
	
	// sampling
	float fragDepth = max(min(getRealDepthFromQuad(texelFetch(uDepth, ivec2(gl_FragCoord), 0).r)/2.-0.5, 1.), 0.);
	float depthDiff = 0.;
	if(fragDepth < 1.) {
		int kernelSize = 2;
		for (int i=-kernelSize; i<=kernelSize; i++) {
			for (int j=-kernelSize; j<=kernelSize; j++) {
				if(i != 0 && j != 0) {
					depthDiff += abs(fragDepth-max(min(getRealDepthFromQuad(texelFetch(uDepth, ivec2(gl_FragCoord) + ivec2(i,j), 0).r)/2.-0.5, 1.), 0.));
				}
			}
		}
	}
	
	retColor = vec3(0., depthDiff, 0.);
	
	return retColor;
}

vec3 getSSAO() {
	vec3 retColor = vec3(0.);
	float fragDepth = getRealDepthFromQuad(texelFetch(uDepth, ivec2(gl_FragCoord), 0).r);
	if(fragDepth < 5.) {
		float depthDiff = 0.;
		int kernelSize = 3;
		int sampleCount = 0;
		for (int i=-kernelSize; i<=kernelSize; i++) {
			for (int j=-kernelSize; j<=kernelSize; j++) {
				if(i != 0 && j != 0) {
					float diff = fragDepth-getRealDepthFromQuad(texelFetch(uDepth, ivec2(gl_FragCoord) + ivec2(i,j), 0).r);
					if(diff > 0. && diff < 0.1) {
						depthDiff += diff;
						++sampleCount;
					}
				}
			}
		}
		
		if(sampleCount > 0) {
			depthDiff = depthDiff/sampleCount;
		}
		depthDiff = min(10.*depthDiff, 1);
		retColor = vec3(1.-depthDiff);
	}
	
	return retColor;
}

vec3 getZoom() {
	vec3 retColor = vec3(0.);
	ivec2 fragCoord = ivec2(gl_FragCoord.xy - uWinSize/2);
	fragCoord = ivec2(int(fragCoord.x/1.5), int(fragCoord.y/1.5));
	fragCoord += uWinSize/2;
	retColor = texelFetch(uFinal, fragCoord, 0).rgb;
	
	return retColor;
}

vec3 getHeatDetector() {
	vec3 retColor = vec3(0.);
	
	float fragDepth = getRealDepthFromQuad(texelFetch(uDepth, ivec2(gl_FragCoord), 0).r);
	if(fragDepth < 5.) {
		int goodSampleCount = 0;
		float sampleDepth = 100.;
		int kernelSize = 10;
		int step = 2;
		for (int i=-step*kernelSize; i<=step*kernelSize; i+=step) {
			for (int j=-step*kernelSize; j<=step*kernelSize; j+=step) {
				sampleDepth = getRealDepthFromQuad(texelFetch(uDepth, ivec2(gl_FragCoord) + ivec2(i, j), 0).r);
				if(sampleDepth < 5.) {
					++goodSampleCount;
				}
			}
		}
		
		float heatCoef = float(goodSampleCount)/float((2*kernelSize+1)*(2*kernelSize+1));
		retColor = HSLtoRGB(int(225.*(1-heatCoef)), 1., 0.5);
	}
	
	return retColor;
}

vec3 getFreaky() {
	vec3 retColor = vec3(0.);
	
	vec3 freakyLight = vec3(0., 1., 0.);
	vec3 normalColor = texelFetch(uNormal, ivec2(gl_FragCoord), 0).xyz;
	vec3 normal = -normalize(normalColor);
	float coef = dot(normal, freakyLight);
	float depth = max(min(getRealDepth(texelFetch(uDepth, ivec2(gl_FragCoord), 0).r) - 5., 1.), 0.);
	
	retColor = vec3((normalColor*0.2 + 0.8*coef)*(1-depth));
	
	return retColor;
}

// MAIN
void main() {
	vec3 color =  vec3(0.9, 0.6, 0.1);
	if(vUV.x > 0.01 && vUV.x < 1.-0.01 && vUV.y > 0.01 && vUV.y < 1.-0.01) {
		if(uID == 1) { color = getSSAO(); }
		if(uID == 2) { color = getToonFilter(); }
		if(uID == 3) { color = getFreaky(); }
		if(uID == 4) { color = getSaturatedFilter(); }
		if(uID == 5) { color = getInvertedFilter(); }
		if(uID == 6) { color = getZoom(); }
		if(uID == 7) { color = getHeatDetector(); }
		if(uID == 8) { color = getXRayFilter(); }
	}	
	
	fragColor = vec4(color, 1.);
}