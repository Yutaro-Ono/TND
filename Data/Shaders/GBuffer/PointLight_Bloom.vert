//-----------------------------------------+
// �|�C���g���C�g�V�F�[�_
//-----------------------------------------+
#version 420
// attribute (���_���W�݂̂ŗǂ�)
layout (location = 0) in vec3 a_pos;

// uniform�o�b�t�@�u���b�N
// 0.�s��
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};

// uniform
uniform mat4 u_worldTransform;

// �t���O�����g�ւ̏o��
out vec2 TexCoords;

void main()
{
	// ���W�ϊ�
	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform * u_view * u_projection;
	gl_Position = pos;

	// ���K���f�o�C�X���W��gBuffer�̃e�N�X�`�����W�ɕϊ�
	// pos��x, y��w�ŏ��Z���A�e�N�X�`�����W��[0.0 �` 1.0]�֕ϊ�
	TexCoords.x = pos.x / pos.w * 0.5f + 0.5f;
	TexCoords.y = pos.y / pos.w * 0.5f + 0.5f;

}