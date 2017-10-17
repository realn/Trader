#version 330

in vec3 vFragNormal;
in vec4 vFragColor;

out vec4 vOutColor;

void main() {
	vOutColor = vFragColor;
}