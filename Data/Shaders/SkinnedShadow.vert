//----------------------------------------------------+
// �X�L�����胁�b�V���p���_�z��
//----------------------------------------------------+
#version 330 core
// Attribute
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in uvec4 inSkinBones;
layout(location = 3) in vec4 inSkinWeights;
layout(location = 4) in vec2 inTexCoord;

// �ϊ��s�� (���[���h�A�r���[&�v���W�F�N�V����)
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
// �s��p���b�g
uniform mat4 uMatrixPalette[196];
// ���C�g��ԍs��
uniform mat4 uLightSpaceMatrix;

// ���_�ւ̏o��
out vec2 fragTexCoord;        // �e�N�X�`�����W
out vec3 fragNormal;          // ���[���h�X�y�[�X��̖@��
out vec3 fragWorldPos;        // ���[���h�X�y�[�X��̒��_���W
out vec4 fragPosLightSpace;      // ���C�g�X�y�[�X��̍��W

void main()
{
	// ���_���W���l�����ɕϊ�
	vec4 pos = vec4(inPosition, 1.0);
	
	// �X�L���ʒu���Z�o
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// �X�L���ʒu�����[���h���W�ϊ�
	skinnedPos = skinnedPos * uWorldTransform;
	// ���[���h��̃X�L���ʒu��ۑ�
	fragWorldPos = skinnedPos.xyz;
	// �N���b�v��Ԃ֒��_��ϊ����o�͂ɓn��
	gl_Position = skinnedPos * uViewProj;

	// �X�L���ʒu�̖@�����Z�o
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x
		+ (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y
		+ (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z
		+ (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;
	// �@�������[���h��ԏ�ɕϊ�(w = 0)
	fragNormal = (skinnedNormal * uWorldTransform).xyz;

	// �e�N�X�`�����W���o��
	fragTexCoord = inTexCoord;

	fragPosLightSpace = vec4(fragWorldPos, 1.0) * uLightSpaceMatrix;

}

