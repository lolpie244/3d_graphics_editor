#version 330

uniform int u_ObjectIndex;

out uvec3 FragColor;

flat in int v_Id;
void main()
{
	FragColor = uvec3(u_ObjectIndex, v_Id, 0);

	// vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
 //   	if (dot(circCoord, circCoord) < 1.0) {
	// 	FragColor = uvec3(u_ObjectIndex, gl_PointCoord * 100);
	// } else {
	//    FragColor = uvec3(u_ObjectIndex, gl_PointCoord * 100);
	// }

}
