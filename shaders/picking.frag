#version 330


out uvec3 FragColor;

flat in int v_VertexId;

uniform int u_ObjectId;
uniform int u_Data;

void main()
{
	FragColor = uvec3(u_ObjectId, v_VertexId, u_Data);
}
