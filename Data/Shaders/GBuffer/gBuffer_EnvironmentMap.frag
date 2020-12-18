//-----------------------------------------------------+
// ���}�b�s���O (�}���`�����_�[�Ή�)
//-----------------------------------------------------+
#version 330 core
// �e�o�b�t�@�ւ̏o�� (GBuffer)
layout (location = 0) out vec3 out_gPos;
layout (location = 1) out vec3 out_gNormal;
layout (location = 2) out vec4 out_gAlbedoSpec;

// ���_�V�F�[�_����̓���
in VS_OUT
{
	vec3 normal;
	vec3 fragWorldPos;
}fs_in;

uniform vec3 u_viewPos;
uniform samplerCube u_skybox;     // �L���[�u�}�b�v(�X�J�C�{�b�N�X)

uniform float u_luminance = 0.0f;

void main()
{
	float ratio = 1.00 / 1.52;      // ���˗�
	vec3 I = normalize(fs_in.fragWorldPos - u_viewPos);        // �J�����̌����x�N�g��
	vec3 R = refract(I, normalize(fs_in.normal), ratio);       // �J�����̌����x�N�g���Ɩ@�����甽�˃x�N�g���𐶐�
	// ���ߓx
	float alpha = 1.0f;

	// GBuffer�o��
	out_gPos = fs_in.fragWorldPos;
	out_gNormal = fs_in.normal;
	out_gAlbedoSpec.rgb = texture(u_skybox, R).rgb * u_luminance;
	out_gAlbedoSpec.a = alpha;

}