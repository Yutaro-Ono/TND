//-------------------------------------------------------+
// スカイボックス用頂点シェーダ
//-------------------------------------------------------+
#version 330 core
// attribute
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 uViewMat;
uniform mat4 uProjMat;

void main()
{
	mat4 view = mat4(mat3(uViewMat));
	


	TexCoords = vec3(aPos.y, -aPos.z, aPos.x);
	vec4 pos = uProjMat * view * vec4(aPos, 1.0);
	gl_Position = vec4(-pos.x, -pos.y, pos.w, pos.w);     // z成分は最奥に表示するようにする

}