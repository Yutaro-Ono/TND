//-----------------------------------------------------------------------+
// ���[���h��Ԃɔz�u����UI�p���_�V�F�[�_
//-----------------------------------------------------------------------+
#version 330 core
// attribute
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

// �e�ϊ��s��
uniform mat4 u_WorldTransform;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ViewProj;

// �t���O�����g�ւ̏o�� (�e�N�X�`�����W)
out vec2 FragTexCoords;

void main()
{
	// ���_���W��gl_Position�p��vec4�^�֕ϊ�
	vec4 pos = vec4(a_Pos, 1.0);

	mat4 world = u_WorldTransform;
	mat4 proj = u_Projection;
	mat4 view = u_View;

	pos = pos * world * view * proj;

	// �N���b�v��ԂɃ��[���h���W��ϊ�
	gl_Position = pos;
	
	// �e�N�X�`�����W���o��
	FragTexCoords = a_TexCoords;

}