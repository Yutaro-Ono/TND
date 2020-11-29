#version 330 core
// �o�̓J���[�i�o�̓s�N�Z���J���[�j
out vec4 out_fragColor;

// ���_�V�F�[�_�[����̓���
// �t���O�����g�ւ̏o��
in VS_OUT
{
	vec2 fragTexCoords;          // �e�N�X�`�����W
	vec3 fragNormal;             // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;           // ���[���h�X�y�[�X��̍��W
}fs_in;


// �e�N�X�`���T���v�����O�p�\����
struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	sampler2D depthMap;
};
uniform Material u_mat;

// �f�B���N�V���i�����C�g�p�\����
struct DirectionalLight
{
	// ���C�g����
	vec3 mDirection;
	// �f�B�t���[�Y�F
	vec3 mDiffuseColor;
	// �X�y�L�����[�F
	vec3 mSpecColor;
};

// �e�N�X�`���T���v�����O
uniform sampler2D u_texture;
// ���C�e�B���O�p�ϐ�
// �J�����ʒu�i���[���h��ԁj
uniform vec3 u_viewPos;
// �|���S���\�ʂ̃X�y�L�����[���x
uniform float u_specPower;
// �A���r�G���g���C�g�F
uniform vec3 u_ambientLight;

// �f�B���N�V���i�����C�g
uniform DirectionalLight uDirLight;

void main()
{
	// �|���S���\�ʂ̖@���i�t���O�����g�V�F�[�_�[��ŕ�Ԃ���Ă���j
	vec3 N = normalize(fs_in.fragNormal);
	// �|���S���\�ʂ��烉�C�g�����ւ̃x�N�g��
	vec3 L = normalize(-uDirLight.mDirection);
	// �|���S���\�ʂ���J��������
	vec3 V = normalize(u_viewPos - fs_in.fragWorldPos);
	// -L �x�N�g���� �@�� N �ɑ΂��Ĕ��˂����x�N�g��R�����߂�
	vec3 R = normalize(reflect(-L, N));

	// �t�H�����ˌv�Z
	vec3 Phong = u_ambientLight;
	float NdotL = dot(N, L);

	vec3 Diffuse;
	vec3 Specular;

	Diffuse = uDirLight.mDiffuseColor * max(NdotL,0.0f);
	Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), u_specPower);

	// �ŏI�J���[���o�� (alpha = 1)
	out_fragColor = texture(u_mat.diffuseMap, fs_in.fragTexCoords) * vec4((Diffuse + u_ambientLight),1.0f) + vec4(Specular,1.0f);
}
