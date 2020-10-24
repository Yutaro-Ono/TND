//---------------------------------------------------------------------+
// �t���O�����g�V�F�[�_
// 3D���f��(���b�V��)�`��p�E�@���}�b�v�Ή�
//---------------------------------------------------------------------+
#version 330 core

out vec4 out_fragColor;       // �o�͐�F�J���[

uniform vec3 uCameraPos;

// ���_�V�F�[�_�[����̓��͎󂯎��
in VS_OUT
{
	vec2 TexCoords;
	vec3 FragNormal;
	vec3 FragPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;

}fs_in;

// �}�e���A�����\����
struct Material
{
	sampler2D diffuseMap;     // �f�B�t���[�Y�}�b�v
	sampler2D specularMap;    // �X�y�L�����}�b�v
	sampler2D normalMap;      // �@���}�b�v
	float     shininess;      // ���ˌ��̋���
};

// ���C�g(����)���\����
struct Light
{
	vec3 mPosition;
	vec3 mDirection;
	float cutoff;
	float outercutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 mSpecColor;

	float constant;
	float linear;
	float quadratic;
};

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

uniform Material u_mat;
uniform Light u_light;
// �f�B���N�V���i�����C�g
uniform DirectionalLight uDirLight;

void main()
{
	// �͈�[0, 19�̖@���}�b�v�x�N�g�����擾
	vec3 normal = texture(u_mat.normalMap, fs_in.TexCoords).rgb;
	// �@���x�N�g���͈̔͂�[-1, +1]�͈̔͂ɕ�������(�^���W�F���g�X�y�[�X�ɕϊ�)
	normal = normalize(normal * 2.0 - 1.0);


	// diffuseColor
	vec3 color = texture(u_mat.diffuseMap, fs_in.TexCoords).rgb;
	// ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    //vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
	//vec3 diffuse =uDirLight.mDiffuseColor * max(dot(uDirLight.mDirection, normal), 0.0) * color;

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
		// �|���S���\�ʂ̖@���i�t���O�����g�V�F�[�_�[��ŕ�Ԃ���Ă���j
	vec3 N = normalize(fs_in.FragNormal);
	// �|���S���\�ʂ��烉�C�g�����ւ̃x�N�g��
	vec3 L = normalize(-uDirLight.mDirection);
	// �|���S���\�ʂ���J��������
	vec3 V = normalize(uCameraPos - fs_in.FragPos);
	// -L �x�N�g���� �@�� N �ɑ΂��Ĕ��˂����x�N�g��R�����߂�
	vec3 R = normalize(reflect(-L, N));
	// �t�H�����ˌv�Z
	vec3 Phong = ambient;
	float NdotL = dot(N, L);


	vec3 Diffuse = diff * color * max(NdotL, 0.0f);
    vec3 specular = vec3(0.2) * spec;
	// �t���O�����g�J���[���o��
    out_fragColor = vec4((Diffuse + ambient), 1.0) + vec4(specular, 1.0f);
}