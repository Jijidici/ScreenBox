#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

uniform mat4 uMatProjection;
uniform mat4 uMatView;
uniform mat4 uMatModel;

out vec3 vNormal;
out vec2 vUV;

void main() {
	vNormal = (uMatModel * vec4(normal, 0.)).xyz;
	vUV = texcoords;
	gl_Position = uMatProjection * uMatView * uMatModel * vec4(position, 1.);
}