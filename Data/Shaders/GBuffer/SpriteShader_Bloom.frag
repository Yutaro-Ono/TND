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
uniform sampler2D u_texture;     // �e�N�X�`���T���v�����O
uniform float u_intensity = 1.0f;  // �P�x����

void main()
{

	// �e�N�X�`������J���[���T���v�����O
	out_color = texture(u_texture, FragTexCoords);

	vec3 result = out_color.rgb;

	// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
	//float brightness = dot(result, vec3(0.0231, 0.11, 0.65));
	float brightness = dot(result, vec3(0.0, 0.0, 1.0)) * u_intensity;

	if(brightness > 0.5)                                              // �P�x��0.71�𒴂����Ȃ�
	{
		out_brightColor = out_color * 0.5f;
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

}