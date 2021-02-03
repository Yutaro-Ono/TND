//---------------------------------------------+
// ���}�b�s���O (�}���`�����_�[�Ή�)
//---------------------------------------------+
#version 420
// attribute
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

// uniform�o�b�t�@�u���b�N (�s��)
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};

// �o��
// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec3 fragNormal;                // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;          // ���[���h�X�y�[�X��̍��W
}vs_out;

// ����
uniform mat4 u_worldTransform;

void main()
{
	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform;

	vs_out.fragNormal = a_normal * mat3(transpose(u_worldTransform));
	vs_out.fragNormal = vec3(vs_out.fragNormal.y, -vs_out.fragNormal.z, vs_out.fragNormal.x);

	vs_out.fragWorldPos = vec3(pos.y, -pos.z, pos.x);                                                 // ���[���h��̈ʒu�x�N�g�����o��

	gl_Position = pos * u_view * u_projection;
}