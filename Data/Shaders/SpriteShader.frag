//-------------------------------------------------------+
// �X�v���C�g(2D)�p�t���O�����g�V�F�[�_
//-------------------------------------------------------+
#version 330 core

// ���_�V�F�[�_����̓���
in vec2 FragTexCoords;           // �e�N�X�`�����W

// �J���[�o�b�t�@�ւ̏o��
out vec4 outColor;

// uniform
uniform sampler2D u_Texture;     // �e�N�X�`���T���v�����O

void main()
{
	// �e�N�X�`������J���[���T���v�����O
	outColor = texture(u_Texture, FragTexCoords);
}