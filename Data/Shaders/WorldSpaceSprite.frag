//-----------------------------------------------------------------------+
// ���[���h��Ԃɔz�u����UI�p�t���O�����g�V�F�[�_
//-----------------------------------------------------------------------+
#version 330 core

// ���_�V�F�[�_����̓��� (�e�N�X�`�����W)
in vec2 FragTexCoords;

// �o�̓J���[
out vec4 out_FragColor;

// �e�N�X�`���T���v�����O�p
uniform sampler2D u_Texture;

void main()
{
	vec4 color = texture(u_Texture, FragTexCoords);
	out_FragColor = color;
	//out_FragColor.a = 0.1f;
}