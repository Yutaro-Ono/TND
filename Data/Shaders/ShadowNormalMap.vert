#version 330 core

// attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_tangent;

// ���[���h�ϊ��s��E�r���[�v���W�F�N�V�����s��E���C�g�X�y�[�X
uniform mat4 u_worldTransform;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_lightSpaceMatrix;   
// �������W�E�J�������W
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec2 fragTexCoords;              // �e�N�X�`�����W
	vec3 fragNormal;                // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;              // ���[���h�X�y�[�X��̍��W
	vec3 fragViewPos;               // �J�������W
	vec4 fragPosLightSpace;         // ���C�g�X�y�[�X��̍��W
	// �^���W�F���g��ԓ��̊e���W
	vec3 TangentLightPos;           // ���C�g(������)�̍��W
	vec3 TangentViewPos;            // �r���[(�J����)�̍��W
	vec3 TangentFragPos;            // �t���O�����g���W

}vs_out;


void main()
{
	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform;
	vs_out.fragWorldPos     = pos.xyz;
	gl_Position = pos * u_view * u_projection;
	vs_out.fragNormal      =  a_normal * mat3(transpose(inverse(u_worldTransform)));
	vs_out.fragTexCoords   = a_texCoords;
	vs_out.fragViewPos = u_viewPos;
	// ���[���h���W�̒��_�����C�g�X�y�[�X�ɕϊ����ĕۑ�
	vs_out.fragPosLightSpace = vec4(vs_out.fragWorldPos, 1.0) * u_lightSpaceMatrix;


	vec3 T, B, N;     // TBN�s��

	N = a_normal;     // �@������
	T = a_tangent;    // �ڋ��(TangentSpace)����

	// �@���ɑ΂���ڋ�Ԃ̍Ē��s��
	T = normalize(T - dot(T, N) * N);
	// �ڋ�ԃx�N�g���Ɩ@���x�N�g���̊O�ς��琂���x�N�g��B(BiTangent)���擾
	B = cross(N, T);

	T = normalize(vec3(vec4(T, 0.0f) * u_worldTransform));
	B = normalize(vec3(vec4(B, 0.0f) * u_worldTransform));
	N = normalize(vec3(vec4(N, 0.0f) * u_worldTransform));

	// TBN�s����t�s��Ƃ��Đ���
	mat3 TBN = transpose(mat3(T, B, N));

	// �ڋ�ԓ��̍��W��`
	vs_out.TangentLightPos = u_lightPos * TBN;                                     // �ڋ�Ԃɂ���������ʒu
	vs_out.TangentViewPos = u_viewPos * TBN;                                       // �ڋ�Ԃɂ�����r���[���W
	vs_out.TangentFragPos = vec3(vec4(a_pos, 0.0f) * u_worldTransform) * TBN;      // �ڋ�Ԃɂ����钸�_���W



}