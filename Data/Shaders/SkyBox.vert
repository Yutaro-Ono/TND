//-------------------------------------------------------+
// �X�J�C�{�b�N�X�p���_�V�F�[�_
//-------------------------------------------------------+
#version 330 core
// attribute
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 uViewMat;
uniform mat4 uProjMat;

void main()
{
	TexCoords = aPos;
	vec4 pos = uProjMat * uViewMat * vec4(aPos, 1.0);
	gl_Position = pos.xyww;

}