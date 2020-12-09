//-------------------------------------------------------+
// �X�v���C�g(2D)�p���_�V�F�[�_
//-------------------------------------------------------+
#version 330 core

// �A�g���r���[�g (�o�b�t�@��AttribPointer�Ŏw�肵���f�[�^)
layout (location = 0) in vec3 a_Pos;              // ���_���W
layout (location = 1) in vec3 a_Normal;           // �@��
layout (location = 2) in vec2 a_TexCoords;         // �e�N�X�`�����W(UV)

// �t���O�����g�V�F�[�_�ւ̏o��
out vec2 FragTexCoords;

// Uniform�w��q
uniform mat4 u_worldTransform;     // ���[���h�ϊ��s��
uniform mat4 u_viewProj;           // �r���[ * �v���W�F�N�V�����̍����s��

void main()
{
	// ���_���W��gl_Position�p��vec4�^�֕ϊ�
	vec4 pos = vec4(a_Pos, 1.0);
	// �N���b�v��ԂɃ��[���h���W��ϊ�
	gl_Position = pos * u_worldTransform * u_viewProj;

	// �e�N�X�`�����W���t���O�����g�֏o��
	FragTexCoords = a_TexCoords;
}