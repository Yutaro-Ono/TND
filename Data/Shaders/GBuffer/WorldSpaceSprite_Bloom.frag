//-----------------------------------------------------------------------+
// ���[���h��Ԃɔz�u����UI�p�t���O�����g�V�F�[�_
//-----------------------------------------------------------------------+
#version 330 core
// �o�̓J���[
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;
// ���_�V�F�[�_����̓��� (�e�N�X�`�����W)
in vec2 TexCoords;


// �e�N�X�`���T���v�����O�p
uniform sampler2D u_texture;

void main()
{
	vec4 color = texture(u_texture, TexCoords);
	out_color = color;

	// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
	float brightness = dot(out_color.rgb, vec3(0.0231, 0.11, 0.51));

	if(brightness > 0.3f)                                              // �P�x��0.4�𒴂����Ȃ�
	{
		out_brightColor = out_color * 0.3f;
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}