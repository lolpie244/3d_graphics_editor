#version 330 core

layout (location = 0) in vec3 pos;

flat out int v_Id;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_View;

uniform int u_Id;

void main()
{
   gl_Position = u_Projection * u_View * u_Model * vec4(pos, 1.0);
   v_Id = u_Id;
}

