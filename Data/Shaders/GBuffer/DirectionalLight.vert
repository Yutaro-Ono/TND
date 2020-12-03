//-----------------------------------------+
// �f�B���N�V���i�����C�g�V�F�[�_
//-----------------------------------------+
#version 330 core
// attribute (���_���W�݂̂ŗǂ�)
layout (location = 0) in vec2 a_pos;

out vec2 TexCoords;

void main()
{

	vec4 pos = vec4(a_pos, 0.0, 1.0);

	gl_Position = pos;

	// ���K���f�o�C�X���W��gBuffer�̃e�N�X�`�����W�ɕϊ�
	// pos��x, y��w�ŏ��Z���A�e�N�X�`�����W��[0.0 �` 1.0]�֕ϊ�
	TexCoords.x = pos.x / pos.w * 0.5f + 0.5f;
	TexCoords.y = pos.y / pos.w * 0.5f + 0.5f;
}