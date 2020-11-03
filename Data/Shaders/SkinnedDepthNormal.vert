//----------------------------------------------------+
// �X�L�����胁�b�V���p���_�z��
//----------------------------------------------------+
#version 330 core
// Attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in uvec4 a_skinBones;
layout(location = 3) in vec4 a_skinWeights;
layout(location = 4) in vec2 a_texCoords;
layout(location = 5) in vec3 a_tangent;

// �ϊ��s�� (���[���h�A�r���[&�v���W�F�N�V����)
uniform mat4 u_worldTransform;
// �s��p���b�g
uniform mat4 u_matrixPalette[196];
// ���C�g��ԍs��
uniform mat4 u_lightSpaceMatrix;


out vec4 fragPosLightSpace;      // ���C�g�X�y�[�X��̍��W

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

	// �N���b�v��Ԃ֒��_��ϊ����o�͂ɓn��
	gl_Position = skinnedPos * u_lightSpaceMatrix;


}

