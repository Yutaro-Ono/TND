//-----------------------------------------------------------------------+
// ���[���h��Ԃɔz�u����UI�p���_�V�F�[�_
//-----------------------------------------------------------------------+
#version 420
// attribute
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

// uniform�o�b�t�@�u���b�N
// 0.�s��
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};


// �e�ϊ��s��
uniform mat4 u_worldTransform;

// �t���O�����g�ւ̏o�� (�e�N�X�`�����W)
out vec2 TexCoords;

void main()
{
	// ���_���W��gl_Position�p��vec4�^�֕ϊ�
	vec4 pos = vec4(a_pos, 1.0);

	mat4 world = u_worldTransform;
	mat4 proj = u_projection;
	mat4 view = u_view;

	pos = pos * world * view * proj;

	// �N���b�v��ԂɃ��[���h���W��ϊ�
	gl_Position = pos;
	
	// �e�N�X�`�����W���o��
	TexCoords = a_texCoords;

}