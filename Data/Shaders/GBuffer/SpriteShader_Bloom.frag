//-------------------------------------------------------+
// �X�v���C�g(2D)�p�t���O�����g�V�F�[�_
//-------------------------------------------------------+
#version 330 core
// �o�̓J���[
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;

// ���_�V�F�[�_����̓���
in vec2 FragTexCoords;           // �e�N�X�`�����W


// uniform
uniform sampler2D u_Texture;     // �e�N�X�`���T���v�����O

void main()
{

	// �e�N�X�`������J���[���T���v�����O
	out_color = texture(u_Texture, FragTexCoords);

	vec3 result = out_color.rgb;

	// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
	float brightness = dot(result, vec3(0.0231, 0.11, 0.51));

	if(brightness > 0.31)                                              // �P�x��0.4�𒴂����Ȃ�
	{
		out_brightColor = out_color * 0.6f;
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

}