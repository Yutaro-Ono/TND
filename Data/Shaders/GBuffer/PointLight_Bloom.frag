//---------------------------------------------------+
// �|�C���g���C�g�V�F�[�_
//---------------------------------------------------+
#version 330 core
// �o��
layout (location = 0) out vec4 out_colorBuffer;
layout (location = 1) out vec4 out_brightColor;
// ���_�V�F�[�_����̓���
in vec2 TexCoords;

// �|�C���g���C�g�\����
struct PointLight
{
	vec3 position;     // �������W
	vec3 ambient;      // ���C�g�A���r�G���g
	vec3 diffuse;      // ���C�g�f�B�t���[�Y
	vec3 specular;     // ���C�g�X�y�L����

	float constant;    // �萔
	float linear;      // ���`��
	float quadratic;   // 2�捀

	float luminance;   // �|�C���g���C�g�P�x
};

// GBuffer�\����
struct GBuffer
{
	sampler2D pos;
	sampler2D normal;
	sampler2D albedoSpec;
	sampler2D emissive;
};

// uniform
uniform PointLight u_pl;
uniform GBuffer u_gBuffer;


uniform vec3 u_viewPos;            // �J�������W
uniform float u_brightLine = 0.1f;       // ���P�x����ƂȂ�

void main()
{
	// gBuffer����e�v�f�𓾂�
	vec3  Position   = texture(u_gBuffer.pos, TexCoords).xyz;
	vec3  Normal     = texture(u_gBuffer.normal, TexCoords).xyz;
	vec4  albedoSpec = texture(u_gBuffer.albedoSpec, TexCoords);
	vec3  Albedo     = albedoSpec.rgb;
	float Spec_p     = albedoSpec.a;

	// ����
	float l_distance = length(u_pl.position - Position);
	// �������̎Z�o
	float attenuation = 1.0f / (u_pl.constant + u_pl.linear * l_distance + u_pl.quadratic * (l_distance * l_distance));

	// �f�B�t���[�Y
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_pl.position - Position);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = u_pl.diffuse * diff * Albedo;

	// �X�y�L����
	vec3 viewDir = normalize(u_viewPos - Position);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfDir), 0.0), 128);
	vec3 specular = u_pl.specular * spec * Spec_p * u_pl.luminance;

	// �A���r�G���g
	vec3 ambient = u_pl.ambient * Albedo * u_pl.luminance;

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;

	// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
	//float brightness = dot(result, vec3(0.1326, 0.1352, 0.142));
	float brightness = dot(result, vec3(0.1326, 0.1352, 0.342));

	if(brightness > u_brightLine)                                              // �P�x��0.4�𒴂����Ȃ�
	{
		out_brightColor = vec4(result, 0.0f);
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	out_colorBuffer = vec4(result, 1.0);

}