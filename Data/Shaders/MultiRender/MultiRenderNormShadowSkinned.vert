//----------------------------------------------------+
// �X�L�����胁�b�V���p���_�V�F�[�_
//----------------------------------------------------+
#version 330 core
// Attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in uvec4 a_skinBones;
layout(location = 3) in vec4 a_skinWeights;
layout(location = 4) in vec2 a_texCoords;
layout(location = 5) in vec3 a_tangent;

// �ϊ��s�� (���[���h�A�r���[�A�v���W�F�N�V����)
uniform mat4 u_worldTransform;
uniform mat4 u_view;
uniform mat4 u_projection;
// �s��p���b�g
uniform mat4 u_matrixPalette[196];
// ���C�g��ԍs��
uniform mat4 u_lightSpaceMatrix;
// �������W�E�J�������W
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;


// �t���O�����g�ւ̏o��
out VS_OUT
{
	vec2 fragTexCoords;          // �e�N�X�`�����W
	vec3 fragNormal;            // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;          // ���[���h�X�y�[�X��̍��W
	vec3 fragViewPos;
	vec4 fragPosLightSpace;     // ���C�g�X�y�[�X��̍��W
	// �^���W�F���g��ԓ��̊e���W
	vec3 TangentLightPos;           // ���C�g(������)�̍��W
	vec3 TangentViewPos;            // �r���[(�J����)�̍��W
	vec3 TangentFragPos;            // �t���O�����g���W

}vs_out;

void main()
{
	// ���_���W���l�����ɕϊ�
	vec4 pos = vec4(a_pos, 1.0);
	
	// �e�N�X�`�����W���o��
	vs_out.fragTexCoords = a_texCoords;
	vs_out.fragViewPos = u_viewPos;

	// �X�L���ʒu���Z�o
	vec4 skinnedPos = (pos * u_matrixPalette[a_skinBones.x]) * a_skinWeights.x;
	skinnedPos += (pos * u_matrixPalette[a_skinBones.y]) * a_skinWeights.y;
	skinnedPos += (pos * u_matrixPalette[a_skinBones.z]) * a_skinWeights.z;
	skinnedPos += (pos * u_matrixPalette[a_skinBones.w]) * a_skinWeights.w;

	// ���[���h��̃X�L���ʒu��ۑ�
	vs_out.fragWorldPos = (skinnedPos * u_worldTransform).xyz;
	// �N���b�v��Ԃ֒��_��ϊ����o�͂ɓn��
	gl_Position = skinnedPos * u_worldTransform * u_view * u_projection;
	// �X�L���ʒu�̖@�����Z�o
	vec4 skinnedNormal = vec4(a_normal, 0.0f);
	skinnedNormal = (skinnedNormal * u_matrixPalette[a_skinBones.x]) * a_skinWeights.x
		+ (skinnedNormal * u_matrixPalette[a_skinBones.y]) * a_skinWeights.y
		+ (skinnedNormal * u_matrixPalette[a_skinBones.z]) * a_skinWeights.z
		+ (skinnedNormal * u_matrixPalette[a_skinBones.w]) * a_skinWeights.w;
	// �@�������[���h��ԏ�ɕϊ�(w = 0)
	vs_out.fragNormal = vec3(skinnedNormal) * mat3(transpose(inverse(u_worldTransform)));

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
	//vs_out.TangentFragPos = vec3(vec4(a_pos, 0.0f) * u_worldTransform) * TBN;      // �ڋ�Ԃɂ����钸�_���W
	vs_out.TangentFragPos = vs_out.fragWorldPos * TBN;      // �ڋ�Ԃɂ����钸�_���W


}

