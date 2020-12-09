//-----------------------------------------------------------------------+
// ���[���h��Ԃɔz�u����UI�p�t���O�����g�V�F�[�_
//-----------------------------------------------------------------------+
#version 330 core
// �o�̓J���[
out vec4 out_fragColor;

// ���_�V�F�[�_����̓��� (�e�N�X�`�����W)
in vec2 TexCoords;


// �e�N�X�`���T���v�����O�p
uniform sampler2D u_texture;

void main()
{
	vec4 color = texture(u_texture, TexCoords);
	out_fragColor = color;
}