//---------------------------------------------------------+
// �|�C���g���C�g�V�F�[�_ (�t���O�����g)
//---------------------------------------------------------+
#version 330 core
// �o�̓J���[
out vec4 out_fragColor;
// ���_�V�F�[�_����̓���
in vec2 TexCoords;

// �|�C���g���C�g�\����
struct PointLight
{
	vec3 position;      // �������W
	
	vec3 diffuse;       // ���C�g�f�B�t���[�Y
	vec3 ambient;       // ���C�g�A���r�G���g
	vec3 specular;      // ���C�g�X�y�L����

	float constant;     // �萔
	float linear;       // ���`��
	float quadratic;    // 2�捀
};

// GBuffer�\����
struct GBuffer
{
	sampler2D gPos;
	sampler2D gNormal;
	sampler2D gAlbedoSpec;
};

// uniform
uniform PointLight u_pointLight;
uniform GBuffer u_gBuffer;

uniform vec3 u_viewPos;        // �J�������W



void main()
{
	// GBuffer�̊e�v�f���T���v�����O���ĐF�������o��
	vec3 Position = texture(u_gBuffer.gPos, TexCoords).xyz;
	vec3 Normal = texture(u_gBuffer.gNormal, TexCoords).xyz;
	// �A���x�h�ƃX�y�L�����𕪂��ĕۑ�
	vec4 AlbedoSpec = texture(u_gBuffer.gAlbedoSpec, TexCoords);
	vec3 Albedo = AlbedoSpec.rgb;
	float Spec_p = AlbedoSpec.a;

	// ���_�ƃ|�C���g���C�g�Ƃ̋������v�Z
	float l_distance = length(u_pointLight.position - Position);
	float attenuation = 1.0 / (u_pointLight.constant + u_pointLight.linear * l_distance + u_pointLight.quadratic * (l_distance * l_distance));

	// �f�B�t���[�Y
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_pointLight.position - Position);     // ���C�g�����̎Z�o
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_pointLight.diffuse * diff * Albedo;

	// �X�y�L����
	vec3 viewDir = normalize(u_viewPos - Position);     // �J�����̌������Z�o
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfDir), 0.0), 32);
	vec3 specular = u_pointLight.specular * spec * Spec_p;

	// �A���r�G���g
	vec3 ambient = u_pointLight.ambient * Albedo;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// �ŏI�o��
	vec3 result = ambient + diffuse + specular;
	out_fragColor = vec4(result, 1.0);

}