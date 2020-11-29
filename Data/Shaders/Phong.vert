#version 330 core

// attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

// ���[���h�ϊ��s��E�r���[�v���W�F�N�V�����s��
uniform mat4 u_worldTransform;
uniform mat4 u_viewProj;


// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec2 fragTexCoords;          // �e�N�X�`�����W
	vec3 fragNormal;             // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;           // ���[���h�X�y�[�X��̍��W
}vs_out;

void main()
{
	// vec4�^�ɒ��_���W��ϊ�
	vec4 pos = vec4(a_pos, 1.0);
	// ���_�������[���h�X�y�[�X�ɕϊ�
	pos = pos * u_worldTransform;
	// �t���O�����g�o�͂ɃZ�b�g
	vs_out.fragWorldPos = pos.xyz;
	// �N���b�v��ԂɃ��[���h��̒��_���W��ϊ�
	gl_Position = pos * u_viewProj;

	// �@���������[���h�X�y�[�X�ɕϊ� (w = 0)
	vs_out.fragNormal = (vec4(a_normal, 0.0f) * u_worldTransform).xyz;

	// �e�N�X�`�����W���t���O�����g�V�F�[�_�֏o��
	vs_out.fragTexCoords = a_texCoords;
}
