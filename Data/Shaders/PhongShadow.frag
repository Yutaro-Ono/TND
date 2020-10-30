#version 330 core

// ���_�V�F�[�_�[����̓���

in vec2 fragTexCoord;              // �e�N�X�`�����W
in vec3 fragNormal;                // �@���i���[���h��ԁj
in vec3 fragWorldPos;              // ���_�ʒu�i���[���h��ԁj
in vec4 fragPosLightSpace;         // ���_�ʒu (���C�g���)

// �o�̓J���[�i�o�̓s�N�Z���J���[�j
out vec4 outColor;

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
	vec3 direction;      // ���C�g����
	vec3 ambient;        // �A���r�G���g
	vec3 diffuse;        // �f�B�t���[�Y�F
	vec3 specular;       // �X�y�L�����[�F
};
// �f�B���N�V���i�����C�g
uniform DirectionalLight u_dirLight;

// ���C�e�B���O�p�ϐ�
// �J�������W
uniform vec3 uCameraPos;
// �|���S���\�ʂ̃X�y�L�����[���x
uniform float uSpecPower;


// �V���h�E�̌v�Z
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // �p�[�X�y�N�e�B�u���Z(���ߊ����o�����߂̌v�Z�����A�����w������1�̂��߈Ӗ��Ȃ�)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // ���K���f�o�C�X���W���e�N�X�`�����W�ƍ��킹�邽�߁A[-1�`1]��[0�`1]�͈̔͂ɕϊ�
    projCoords = projCoords * 0.5 + 0.5;
    // �V���h�E�}�b�v��胉�C�g�ɍł��߂��t���O�����g�̐[�x�l�𓾂�
    float closestDepth = texture(u_mat.depthMap, projCoords.xy).r;
    // ���ݕ`�悵�悤�Ƃ��Ă���t���O�����g�̐[�x�l
    float currentDepth = projCoords.z;
    // �V���h�E���� (1.0:�V���h�E 0.0:�V���h�E�̊O)
    // �o�C�A�X��@���ƃ��C�g�̌������璲������
    float bias = max(0.0001 * (1.0 - dot(normalize(fragNormal), u_dirLight.direction)), 0.0001);
    // ���݂̐[�x���ł��߂��t���O�����g�̐[�x���傫�����1.0�A���������0.0(�e�ɂȂ�)
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    vec2 texelSize = 1.0 / textureSize(u_mat.depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_mat.depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{
	// �|���S���\�ʂ̖@���i�t���O�����g�V�F�[�_�[��ŕ�Ԃ���Ă���j
	vec3 N = normalize(fragNormal);
	// �|���S���\�ʂ��烉�C�g�����ւ̃x�N�g��
	vec3 L = normalize(-u_dirLight.direction);
	// �|���S���\�ʂ���J��������
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// -L �x�N�g���� �@�� N �ɑ΂��Ĕ��˂����x�N�g��R�����߂�
	vec3 R = normalize(reflect(-L, N));
	// �t�H�����ˌv�Z
	vec3 Phong = u_dirLight.ambient;
	float NdotL = dot(N, L);

	// �A���r�G���g�v�Z
	vec3 ambient = u_dirLight.ambient * vec3(texture(u_mat.diffuseMap, fragTexCoord));
	// �f�B�t���[�Y�v�Z
	vec3 norm = normalize(fragNormal);
	float diff = max(dot(norm, -u_dirLight.direction), 0.0);
	//float diff = max(NdotL, 0.0);
	vec3 diffuse = u_dirLight.diffuse * diff * vec3(texture(u_mat.diffuseMap, fragTexCoord));

	// �X�y�L�����v�Z
	vec3 viewDir = normalize(uCameraPos - fragWorldPos);
	vec3 reflectDir = reflect(u_dirLight.direction, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	//float spec = pow(max(0.0, dot(R, V)), uSpecPower);
	vec3 specular = u_dirLight.specular * spec * vec3(texture(u_mat.specularMap, fragTexCoord));

	// �e�����̎Z�o
	float shadow = ShadowCalculation(fragPosLightSpace);
	// �V���h�E�̋t�������A0 = �e�̎��Ƀf�B�t���[�Y�ƃX�y�L�����̒l���L�����Z�������(�e�ƂȂ�)
	vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);

	// �ŏI�J���[���o��
	outColor = vec4(result, 1.0);
}
