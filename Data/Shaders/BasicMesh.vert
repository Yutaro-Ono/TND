#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;


uniform mat4 u_worldTransform;
uniform mat4 u_viewProj;

out vec2 fragTexCoord;

void main()
{
	
	vec4 pos = vec4(a_pos, 1.0);
	
	gl_Position = pos * u_worldTransform * u_viewProj;

	fragTexCoord = a_texCoords;
}
