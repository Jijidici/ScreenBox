#version 330

in vec2 vUV;

uniform sampler2D uMaterial;
uniform sampler2D uNormal;
uniform sampler2D uDepth;
uniform vec3 uCameraPosition;
uniform vec3 uLightPosition;
uniform vec3 uLightTarget;
uniform vec3 uLightColor;
uniform float uLightIntensity;
uniform float uLightLength;
uniform mat4 uInverseViewProjection;

out vec4 fragColor;

void main() {
	vec3 diff = texture2D(uMaterial, vUV).rgb;
	float spec = texture2D(uMaterial, vUV).a;
	vec3 normal = texture2D(uNormal, vUV).xyz;
	float depth = texture2D(uDepth, vUV).r;

	//compute position
	vec2 xy = vUV*2. -1.;
	vec4 wPosition = vec4(xy, depth*2. -1., 1.) * uInverseViewProjection;
	vec3 position = vec3(wPosition/wPosition.w);
	
	//compute point light
	vec3 l =  normalize(uLightPosition - position);
	vec3 v = position - uCameraPosition;
	vec3 h = normalize(l-v);
	float n_dot_l = clamp(dot(normal, l), 0, 1.0);
	float n_dot_h = clamp(dot(normal, h), 0, 1.0);

	vec3 lightDirection = -normalize(uLightTarget - uLightPosition);
	float cos_theta = dot(l, lightDirection);
	float cos_phi = cos(radians(30.));
	float cos_psy = cos(radians(max(15. - 3., 0.)));

	float falloff = 0.;
	if(cos_theta > cos_phi) {
		falloff = clamp(pow((cos_theta - cos_phi)/(cos_psy - cos_phi), 4.), 0., 1.);
	}
	
	
	float d = distance(uLightPosition, position);
	float att = 1.;
	if(d > uLightLength) {
		att = uLightLength/ (d*d);
	}

	vec3 color = falloff * att * uLightColor * uLightIntensity * (diff * n_dot_l + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
	//vec3 color = vec3(l);

	fragColor = vec4(color, 1.);
}