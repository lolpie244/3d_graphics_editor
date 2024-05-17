#version 330


out uvec3 FragColor;

flat in int v_VertexId;

uniform int u_ObjectId;
uniform int u_Data;

void main()
{
	FragColor = uvec3(0, 0, 0);
}
