//---------------------------------------------------------------------+
// �t���O�����g�V�F�[�_
// 3D���f��(���b�V��)�`��p�E�@���}�b�v�Ή�
//---------------------------------------------------------------------+
#version 330 core

out vec4 out_fragColor;       // �o�͐�F�J���[

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


uniform Material u_mat;


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
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
	// �t���O�����g�J���[���o��
    out_fragColor = vec4(ambient + diffuse + specular, 1.0);
}