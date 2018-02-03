#version 330

in vec3 vFragNormal;
in vec4 vFragColor;

out vec4 vOutColor;

uniform vec4 vMeshColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main() {
	vOutColor = vFragColor * vMeshColor;
}