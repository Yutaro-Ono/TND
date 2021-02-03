//-------------------------------------------------------------------------+
// GBuffer�E�Ԃ̃��f���p ���_�V�F�[�_
// ���}�b�v�E���ˌv�Z
//-------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

// uniform�o�b�t�@�u���b�N
// 0.�s��
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};
// 1.�J�������W
layout(std140, binding = 1) uniform CameraVariable
{
	vec3 u_viewPos;
};

// ���[���h�ϊ��s��E�r���[�v���W�F�N�V�����s��E���C�g�X�y�[�X
uniform mat4 u_worldTransform;
//uniform mat4 u_view;
//uniform mat4 u_projection;
uniform mat4 u_lightSpaceMatrix;   
// �������W�E�J�������W
uniform vec3 u_lightPos;

// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec2 fragTexCoords;              // �e�N�X�`�����W
	vec3 fragNormal;                // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;              // ���[���h�X�y�[�X��̍��W
	vec3 fragViewPos;               // �J�������W
	vec4 fragPosLightSpace;         // ���C�g�X�y�[�X��̍��W
}vs_out;


void main()
{
	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform;
	gl_Position = pos * u_view * u_projection;
	vs_out.fragWorldPos     = pos.xyz;
	vs_out.fragNormal      =  a_normal * mat3(transpose(inverse(u_worldTransform)));
	vs_out.fragTexCoords   = a_texCoords;
	vs_out.fragViewPos = u_viewPos;
	// ���[���h���W�̒��_�����C�g�X�y�[�X�ɕϊ����ĕۑ�
	vs_out.fragPosLightSpace = vec4(vs_out.fragWorldPos, 1.0) * u_lightSpaceMatrix;

	vs_out.fragNormal = vec3(vs_out.fragNormal.y, -vs_out.fragNormal.z, vs_out.fragNormal.x);
	vs_out.fragWorldPos = vec3(pos.y, -pos.z, pos.x);                                                 // ���[���h��̈ʒu�x�N�g�����o��

}