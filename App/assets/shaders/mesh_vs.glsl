#version 330

in vec3 vInVertex;
in vec3 vInNormal;
in vec4 vInColor;

uniform mat4 mTransform;

out vec3 vFragNormal;
out vec4 vFragColor;

void main() {
	gl_Position = mTransform * vec4(vInVertex, 1.0f);
	vFragNormal = vInNormal;
	vFragColor = vInColor;
}