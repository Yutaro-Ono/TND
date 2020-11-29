//----------------------------------------------------+
// �X�L�����b�V�����_�V�F�[�_ (�V���h�E�p)
//----------------------------------------------------+
#version 330 core
// Attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in uvec4 a_skinBones;
layout(location = 3) in vec4 a_skinWeights;
layout(location = 4) in vec2 a_texCoords;

// �ϊ��s�� (���[���h�A�r���[�A�v���W�F�N�V����)
uniform mat4 u_worldTransform;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_viewProj;
// �s��p���b�g
uniform mat4 u_matrixPalette[196];
// ���C�g��ԍs��
uniform mat4 u_lightSpaceMatrix;

// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec2 fragTexCoords;          // �e�N�X�`�����W
	vec3 fragNormal;             // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;           // ���[���h�X�y�[�X��̍��W
	vec4 fragPosLightSpace;      // ���C�g�X�y�[�X��̍��W
}vs_out;

void main()
{
	// ���_���W���l�����ɕϊ�
	vec4 pos = vec4(a_pos, 1.0);
	
	// �X�L���ʒu���Z�o
	vec4 skinnedPos = (pos * u_matrixPalette[a_skinBones.x]) * a_skinWeights.x;
	skinnedPos += (pos * u_matrixPalette[a_skinBones.y]) * a_skinWeights.y;
	skinnedPos += (pos * u_matrixPalette[a_skinBones.z]) * a_skinWeights.z;
	skinnedPos += (pos * u_matrixPalette[a_skinBones.w]) * a_skinWeights.w;

	// �X�L���ʒu�����[���h���W�ϊ�
	skinnedPos = skinnedPos * u_worldTransform;
	// ���[���h��̃X�L���ʒu��ۑ�
	vs_out.fragWorldPos = skinnedPos.xyz;
	// �N���b�v��Ԃ֒��_��ϊ����o�͂ɓn��
	gl_Position = skinnedPos * u_view * u_projection;
	// �X�L���ʒu�̖@�����Z�o
	vec4 skinnedNormal = vec4(a_normal, 0.0f);
	skinnedNormal = (skinnedNormal * u_matrixPalette[a_skinBones.x]) * a_skinWeights.x
		+ (skinnedNormal * u_matrixPalette[a_skinBones.y]) * a_skinWeights.y
		+ (skinnedNormal * u_matrixPalette[a_skinBones.z]) * a_skinWeights.z
		+ (skinnedNormal * u_matrixPalette[a_skinBones.w]) * a_skinWeights.w;
	// �@�������[���h��ԏ�ɕϊ�(w = 0)
	vs_out.fragNormal = (skinnedNormal * u_worldTransform).xyz;

	// �e�N�X�`�����W���o��
	vs_out.fragTexCoords = a_texCoords;

	vs_out.fragPosLightSpace = vec4(vs_out.fragWorldPos, 1.0) * u_lightSpaceMatrix;

}

