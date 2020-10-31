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
// �s��p���b�g
uniform mat4 uMatrixPalette[196];
// ���C�g��ԍs��
uniform mat4 uLightSpaceMatrix;


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

	// �N���b�v��Ԃ֒��_��ϊ����o�͂ɓn��
	gl_Position = skinnedPos * uLightSpaceMatrix;


}

