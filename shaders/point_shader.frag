#version 330 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform vec4 u_Color;
void main()
{
	FragColor = u_Color;

	vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
	if (dot(circCoord, circCoord) > 1.0) {
		discard;
	}
}