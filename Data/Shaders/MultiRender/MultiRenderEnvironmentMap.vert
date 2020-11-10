//---------------------------------------------+
// ���}�b�s���O (�}���`�����_�[�Ή�)
//---------------------------------------------+
#version 330 core
// attribute
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

// �o��
// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec3 normal;            // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;          // ���[���h�X�y�[�X��̍��W
}vs_out;

// ����
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;

void main()
{
	vec4 pos = vec4(a_pos, 1.0) * u_modelMat;

	vs_out.normal = a_normal * mat3(transpose(u_modelMat));

	vs_out.normal = vec3(vs_out.normal.y, -vs_out.normal.z, vs_out.normal.x);

	vs_out.fragWorldPos = vec3(pos.y, -pos.z, pos.x);                                                 // ���[���h��̈ʒu�x�N�g�����o��

	gl_Position = pos * u_viewMat * u_projMat;
}