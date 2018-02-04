#version 330

in vec3 vFragNormal;
in vec4 vFragColor;

out vec4 vOutColor;

uniform vec4 vMeshColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

void main() {
	vOutColor = mix(vFragColor, vMeshColor, vMeshColor.a);
}