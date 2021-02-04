//---------------------------------------------------+
// �X�|�b�g���C�g�E�t���O�����g�V�F�[�_
// �o��:�W���J���[�o�b�t�@�E���P�x�o�b�t�@
//---------------------------------------------------+
#version 330 core
// �o��
layout (location = 0) out vec4 out_colorBuffer;
layout (location = 1) out vec4 out_brightColor;
// ���_�V�F�[�_����̓���
in vec2 TexCoords;

// �X�|�b�g���C�g�\����
struct SpotLight
{
	vec3 position;     // �������W
	vec3 direction;    // ����
	vec3 ambient;      // ���C�g�A���r�G���g
	vec3 diffuse;      // ���C�g�f�B�t���[�Y
	vec3 specular;     // ���C�g�X�y�L����

	float cutOff;      // �J�b�g�I�t
	float outerCutOff;

	float constant;    // �萔
	float linear;      // ���`��
	float quadratic;   // 2�捀

	float luminance;   // �P�x
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
uniform SpotLight u_spotL;
uniform GBuffer u_gBuffer;

uniform vec3 u_viewPos;            // �J�������W

void main()
{
	// gBuffer����e�v�f�𓾂�
	vec3  Position   = texture(u_gBuffer.pos, TexCoords).xyz;
	vec3  Normal     = texture(u_gBuffer.normal, TexCoords).xyz;
	vec4  albedoSpec = texture(u_gBuffer.albedoSpec, TexCoords);
	vec3  Albedo     = albedoSpec.rgb;
	float Spec_p     = albedoSpec.a;

	vec3 lightDir = normalize(u_spotL.position - Position);
	float theta = dot(lightDir, normalize(-u_spotL.direction));



	if(theta > u_spotL.cutOff)
	{
		// �A���r�G���g
		vec3 ambient = u_spotL.ambient * Albedo;
		// �f�B�t���[�Y
		vec3 norm = normalize(Normal);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = u_spotL.diffuse * diff * Albedo * u_spotL.luminance;

		// �X�y�L����
		vec3 viewDir = normalize(u_viewPos - Position);
		vec3 halfDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfDir), 0.0), 128);
		vec3 specular = u_spotL.specular * spec * Spec_p;


		// ����
		float l_distance = length(u_spotL.position - Position);
		// �������̎Z�o
		float attenuation = 1.0f / (u_spotL.constant + u_spotL.linear * l_distance + u_spotL.quadratic * (l_distance * l_distance));
		//ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular + texture(u_gBuffer.emissive, TexCoords).rgb;

		// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
		float brightness = dot(result, vec3(0.1326, 0.3352, 0.442));

		if(brightness > 0.2)                                              // �P�x��0.4�𒴂����Ȃ�
		{
			out_brightColor = vec4(result, Spec_p);
		}
		else
		{
			out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		out_colorBuffer = vec4(result, 1.0);
	}
	else
	{
		out_colorBuffer = vec4(u_spotL.ambient * Albedo, Spec_p);

		// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
		float brightness = dot(u_spotL.ambient * Albedo, vec3(0.1326, 0.3352, 0.442));

		if(brightness > 0.2)                                              // �P�x��0.4�𒴂����Ȃ�
		{
			out_brightColor = vec4(u_spotL.ambient * Albedo, Spec_p);
		}
		else
		{
			out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}







}