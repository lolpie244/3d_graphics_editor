#version 330 core

out vec4 FragColor;
in vec2 v_TexCoord;

uniform vec4 u_Color;
void main()
{
	FragColor = u_Color;
}
