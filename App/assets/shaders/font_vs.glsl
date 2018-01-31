#version 330

in vec2 vInVertex;
in vec2 vInTCoord;
in vec4 vInColor;

out vec2 vVFTCoord;
out vec4 vVFColor;

uniform mat4 mTransform;

void main() {
	vVFColor = vInColor;
	vVFTCoord = vInTCoord;
	gl_Position = mTransform * vec4(vInVertex, 0, 1);
}