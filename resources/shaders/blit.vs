#version 330

layout(location = 0) in vec3 position;

out vec2 vUV;

void main() {
	vUV = position.xy + 0.5;
	gl_Position = vec4(2.*position.xy, 0., 1.);
}