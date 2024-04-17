#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Transform;

void main()
{
   gl_Position = u_Transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   v_TexCoord = texCoord;
}
