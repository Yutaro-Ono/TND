#version 330 core

// attribute
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// ���[���h�ϊ��s��E�r���[�v���W�F�N�V�����s��
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;


// �t���O�����g�ւ̏o��
// �e�N�X�`�����W
out vec2 fragTexCoord;
// ���[���h�X�y�[�X��̖@��
out vec3 fragNormal;
// ���[���h�X�y�[�X��̍��W
out vec3 fragWorldPos;

void main()
{
	// vec4�^�ɒ��_���W��ϊ�
	vec4 pos = vec4(inPosition, 1.0);
	// ���_�������[���h�X�y�[�X�ɕϊ�
	pos = pos * uWorldTransform;
	// �t���O�����g�o�͂ɃZ�b�g
	fragWorldPos = pos.xyz;
	// �N���b�v��ԂɃ��[���h��̒��_���W��ϊ�
	gl_Position = pos * uViewProj;

	// �@���������[���h�X�y�[�X�ɕϊ� (w = 0)
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// �e�N�X�`�����W���t���O�����g�V�F�[�_�֏o��
	fragTexCoord = inTexCoord;
}
