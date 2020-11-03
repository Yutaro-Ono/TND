//---------------------------------------------------------------------+
// ���_�V�F�[�_
// 3D���f��(���b�V��)�`��p�E�@���}�b�v�Ή�
// �ڋ��(TangentSpace�Ƃ��Ċe���W���`���A�t���O�����g�֏o��)
//---------------------------------------------------------------------+
#version 330 core
// �A�g���r���[�g (VertexAttribPointer�ɂĐݒ�)
layout (location = 0) in vec3 a_pos;              // ���_���W
layout (location = 1) in vec3 a_normal;           // �@���x�N�g��
layout (location = 2) in vec2 a_texCoords;        // �e�N�X�`��UV���W
layout (location = 3) in vec3 a_tangent;          // �ڃx�N�g��

// uniform �ϐ�
uniform mat4 u_viewProj;              // �r���[�s��E�v���W�F�N�V�����s��
uniform mat4 u_worldTransform;                // ���f���s��

uniform vec3 u_lightPos;             // �����ʒu
uniform vec3 u_viewPos;              // �J�����̎��_�x�N�g��

// out�\���� (�t���O�����g�֏o�͂���ϐ����܂Ƃ߂��\����)
out VS_OUT
{
	vec2 TexCoords;                 // �e�N�X�`�����W
	vec3 FragNormal;
	vec3 FragPos;
	// �^���W�F���g��ԓ��̊e���W
	vec3 TangentLightPos;           // ���C�g(������)�̍��W
	vec3 TangentViewPos;            // �r���[(�J����)�̍��W
	vec3 TangentFragPos;            // �t���O�����g���W

}vs_out;


void main()
{
	
	vec4 pos = vec4(a_pos, 1.0);
	pos = pos * u_worldTransform;
	// ���W�ϊ����s������ŁAGL�̑g�ݍ��ݕϐ��ɑ��
	gl_Position = pos * u_viewProj;
	vs_out.FragPos = pos.xyz;
	// UV���W���t���O�����g�V�F�[�_�ɑ���
	vs_out.TexCoords = a_texCoords;
	vs_out.FragNormal = (vec4(a_normal, 0.0f) * u_worldTransform).xyz;


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