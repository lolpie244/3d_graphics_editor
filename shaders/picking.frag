#version 330

uniform int u_ObjectIndex;

out uvec3 FragColor;

flat in int v_Id;
void main()
{
	FragColor = uvec3(u_ObjectIndex, v_Id, 0);
}
