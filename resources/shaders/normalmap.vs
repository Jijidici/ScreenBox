#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 uMatProjection;
uniform mat4 uMatView;
uniform mat4 uMatModel;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vUV;
out vec3 vTangent;
out vec3 vBitangent;

void main() {
	vPosition = (uMatModel * vec4(vPosition, 0.)).xyz;
	vNormal = (uMatModel * vec4(normal, 0.)).xyz;
	vUV = texcoords;
	vTangent = (uMatModel * vec4(tangent, 0.)).xyz;
	vBitangent = (uMatModel * vec4(bitangent, 0.)).xyz;
	gl_Position = uMatProjection * uMatView * uMatModel * vec4(position, 1.);
}