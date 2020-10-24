//---------------------------------------------------------------------+
// ���_�V�F�[�_
// 3D���f��(���b�V��)�`��p�E�@���}�b�v�Ή�
// �ڋ��(TangentSpace�Ƃ��Ċe���W���`���A�t���O�����g�֏o��)
//---------------------------------------------------------------------+
#version 330 core
// �A�g���r���[�g (VertexAttribPointer�ɂĐݒ�)
layout (location = 0) in vec3 in_Position;              // ���_���W
layout (location = 1) in vec3 in_Normal;           // �@���x�N�g��
layout (location = 2) in vec2 in_TexCoords;        // �e�N�X�`��UV���W
layout (location = 3) in vec3 in_Tangent;          // �ڃx�N�g��

// uniform �ϐ�
uniform mat4 u_view;                 // �r���[�s��
uniform mat4 u_projection;           // �v���W�F�N�V�����s��
uniform mat4 uViewProj;              // �r���[�s��E�v���W�F�N�V�����s��
uniform mat4 uWorldTransform;                // ���f���s��

uniform vec3 uLightPos;             // �����ʒu
uniform vec3 uViewPos;              // �J�����̎��_�x�N�g��

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
	
	vec4 pos = vec4(in_Position, 1.0);
	pos = pos * uWorldTransform;
	// ���W�ϊ����s������ŁAGL�̑g�ݍ��ݕϐ��ɑ��
	gl_Position = pos * uViewProj;
	vs_out.FragPos = pos.xyz;
	// UV���W���t���O�����g�V�F�[�_�ɑ���
	vs_out.TexCoords = in_TexCoords;
	vs_out.FragNormal = (vec4(in_Normal, 0.0f) * uWorldTransform).xyz;


	vec3 T, B, N;     // TBN�s��

	N = in_Normal;     // �@������
	T = in_Tangent;    // �ڋ��(TangentSpace)����

	mat3 normalMatrix = transpose(inverse(mat3(uWorldTransform)));
	T = normalize(normalMatrix * in_Tangent);
	N = normalize(normalMatrix * in_Normal);

	// �@���ɑ΂���ڋ�Ԃ̍Ē��s��
	T = normalize(T - dot(T, N) * N);
	// �ڋ�ԃx�N�g���Ɩ@���x�N�g���̊O�ς��琂���x�N�g��B(BiTangent)���擾
	B = cross(N, T);

	T = normalize(vec3(uWorldTransform * vec4(T, 0.0f)));
	B = normalize(vec3(uWorldTransform * vec4(B, 0.0f)));
	N = normalize(vec3(uWorldTransform * vec4(N, 0.0f)));

	// TBN�s����t�s��Ƃ��Đ���
	mat3 TBN = transpose(mat3(T, B, N));

	// �ڋ�ԓ��̍��W��`
	vs_out.TangentLightPos = TBN * uLightPos;                                  // �ڋ�Ԃɂ���������ʒu
	vs_out.TangentViewPos = TBN * uViewPos;                                    // �ڋ�Ԃɂ�����r���[���W
	vs_out.TangentFragPos = TBN * vec3(uWorldTransform * vec4(in_Position, 0.0f));      // �ڋ�Ԃɂ����钸�_���W
}