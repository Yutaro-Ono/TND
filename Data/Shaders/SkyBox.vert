//-------------------------------------------------------+
// �X�J�C�{�b�N�X�p���_�V�F�[�_
//-------------------------------------------------------+
#version 330 core
// attribute
layout (location = 0) in vec3 a_Pos;

out vec3 TexCoords;

uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	// �e�N�X�`�����W�Ƀ��[�J���ʒu�x�N�g����ݒ�
	TexCoords = vec3(a_Pos.y, -a_Pos.z, a_Pos.x);

	vec4 pos = u_Proj * u_View * vec4(a_Pos, 1.0);

	gl_Position = vec4(-pos.x, -pos.y, pos.w, pos.w);     // z������w�Œu��������(�ŉ��\��)


}