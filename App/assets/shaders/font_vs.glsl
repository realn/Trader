#version 330

in vec2 vInPosition;
in vec2 vInTexCoord;
in vec4 vInColor;

out vec2 vVFTCoord;
out vec4 vVFColor;

uniform mat4 mTransform;

void main() {
	vVFColor = vInColor;
	vVFTCoord = vInTexCoord;
	gl_Position = mTransform * vec4(vInPosition, 0, 1);
}