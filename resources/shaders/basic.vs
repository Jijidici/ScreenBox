#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 uMatProjection = mat4(1.f);
uniform mat4 uMatView = mat4(1.f);
uniform mat4 uMatModel = mat4(1.f);

out vec3 vPosition;
out vec3 vViewSpacePosition;
out vec3 vNormal;
out vec2 vUV;

void main() {
	vPosition = (uMatModel * vec4(position, 1.)).xyz;
	vViewSpacePosition = (uMatView * uMatModel * vec4(position, 1.)).xyz;
	vNormal = (uMatModel * vec4(normal, 0.)).xyz;
	vUV = texcoords;
	gl_Position = uMatProjection * uMatView * uMatModel * vec4(position, 1.);
}