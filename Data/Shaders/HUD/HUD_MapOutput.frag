//------------------------------------------+
// �}�b�vHUD�̃X�N���[���o��
//------------------------------------------+
#version 330 core
// �o�̓J���[
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;
in vec2 fragTexCoord;

// uniform
uniform sampler2D u_texture;     // �e�N�X�`���T���v�����O
uniform float u_intensity = 1.0f;  // �P�x����

// �؂蔲���p�~�`�e�N�X�`��
uniform sampler2D u_mapTex;

void main()
{
	
	// �}�b�v�e�N�X�`���̃T���v�����O
	// ���̃e�N�X�`���̍��F�����݂̂ɕ`���K�p���Ă���
	vec4 mapTexture = texture(u_mapTex, fragTexCoord);

	// �}�b�v��`�悵���ꎞ�ۑ��e�N�X�`���̃J���[�T���v�����O
	vec3 mapColor = texture(u_texture, fragTexCoord).rgb;

	//out_color = texture(u_texture, fragTexCoord);

	vec3 color = vec3(1.0, 1.0, 1.0);
	// �}�b�v�e�N�X�`���̗̈�(����)���̂݃J���[�o��
	if(mapTexture.a == 1.0)
	{
		// �`�挋�ʂ��o��
		out_color = vec4(mapColor, 0.925);

		// �����`�悳��Ă��Ȃ��ꍇ (�m�[�J���[)
		// ���œh��Ԃ�
		if(out_color.a == 0.0)
		{
			out_color = vec4(0.1, 0.1, 1.0, 0.9);
		}
	}
	else
	{
		out_color = vec4(0.0, 0.0, 0.0, 0.0);
	}

	// �}�b�v�̘g���`��
	// �F�l�����ȏ� (���ɋ߂�) �ꍇ�B���X�������Ȃ̂ŉ��P������
	if(mapTexture.r > 0.8f && mapTexture.g > 0.8f && mapTexture.b > 0.8f)
	{
		out_color = mapTexture;
	}

	// ���P�x�o�b�t�@�͎g�p���Ȃ�
	out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

}
