//-----------------------------------------------------+
// ���}�b�s���O (�}���`�����_�[�Ή�)
//-----------------------------------------------------+
#version 330 core
layout (location = 0) out vec4 out_fragColor;
layout (location = 1) out vec4 out_brightColor;


// ���_�V�F�[�_����̓���
in VS_OUT
{
	vec3 normal;
	vec3 fragWorldPos;
}fs_in;

uniform vec3 u_viewPos;
uniform samplerCube u_skybox;     // �L���[�u�}�b�v(�X�J�C�{�b�N�X)

void main()
{
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(fs_in.fragWorldPos - u_viewPos);        // �J�����̌����x�N�g��
	vec3 R = refract(I, normalize(fs_in.normal), ratio);       // �J�����̌����x�N�g���Ɩ@�����甽�˃x�N�g���𐶐�

	vec3 result = texture(u_skybox, R).rgb;

		// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
	float brightness = dot(result, vec3(0.1126, 0.4152, 0.522));     // �P�x���J���[���ʂ̓��ς��狁�߂�

	if(brightness > 0.45)                                              // �P�x��0.5�𒴂����Ȃ�
	{
		out_brightColor = vec4(result, 0.0f);
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	// �ʏ�J���[�o�b�t�@�ւ̏o��
	out_fragColor = vec4(result, 0.8);
}