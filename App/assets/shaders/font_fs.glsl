#version 330

uniform sampler2D texBase;
uniform sampler2D texFont;

in vec2 vVFTCoord;
in vec4 vVFColor;

out vec4 vOutColor;

float checkLevel(float x, float level) {
	return (level <= x && x <= level + 1.0f) ? x - level : 0.0f;
}

void main() {
	float x = vVFColor.a;
	float f = checkLevel(x, 2.0f);	// 2.0f < x < 3.0f
	float b = checkLevel(x, 1.0f);	// 1.0f < x < 2.0f
	float c = checkLevel(x, 0.0f);	// 0.0f < x < 1.0f
	float sf = step(0.001f, f);
	float sb = step(0.001f, b);
	float sc = step(0.001f, c);

	vOutColor = 
		vec4(vVFColor.rgb, c) * sc +
		texture(texBase, vVFTCoord) * vec4(vVFColor.rgb, b) * sb +
		texture(texFont, vVFTCoord) * vec4(vVFColor.rgb, f) * sf;
}