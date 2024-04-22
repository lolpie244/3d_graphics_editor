#version 330

uniform uint u_ObjectIndex;
uniform sampler2D u_Texture;
in vec2 v_TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out uint InstanceId;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	FragColor = texColor;
	InstanceId = u_ObjectIndex;
}
