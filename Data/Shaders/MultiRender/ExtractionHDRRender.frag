//----------------------------------------------------------+
// �}���`�����_�����O�p �F�l�E���P�x���o�V�F�[�_
//----------------------------------------------------------+
#version 330 core
// �e�o�b�t�@�ւ̏o��
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

// ���_�V�F�[�_����̓���
in VS_OUT
{

	vec2 fragTexCoords;

}fs_in;

void main()
{
	vec3 result = vec3(texture(

}