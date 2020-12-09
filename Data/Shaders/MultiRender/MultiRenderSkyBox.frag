//----------------------------------------------------------+
// �X�J�C�{�b�N�X�p�t���O�����g�V�F�[�_�[
//----------------------------------------------------------+
#version 330 core
// �e�o�b�t�@�ւ̏o�� (�}���`�����_�[�^�[�Q�b�g)
layout (location = 0) out vec4 out_fragColor;
layout (location = 1) out vec4 out_brightColor;

in vec3 TexCoords;

uniform samplerCube u_skybox;

void main()
{
    // �X�J�C�{�b�N�X�p�e�N�X�`�����T���v�����O
	vec4 result = texture(u_skybox, TexCoords);
	// �ʏ�J���[�o�b�t�@�ւ̏o��
	out_fragColor = result;
	
	// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
	float brightness = dot(result.xyz, vec3(0.2126, 0.7152, 0.0722));     // �P�x���J���[���ʂ̓��ς��狁�߂�
	
	if(brightness > 3.0)                                              // �P�x��0.5�𒴂����Ȃ�
	{
		out_brightColor = vec4(result.xyz, 0.0f);
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}


}