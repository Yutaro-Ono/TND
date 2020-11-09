//---------------------------------------------------+
// �K�E�X�ڂ����V�F�[�_
//---------------------------------------------------+
#version 330 core
// �o�̓J���[
out vec4 out_fragColor;
// ���̓e�N�X�`��
in vec2 TexCoords;

uniform sampler2D u_blurSource;

// �K�E�X�ڂ����̃p�����[�^�\����
struct GaussBlurParam
{
	int sampleCount;
	vec3 offset[16];
};
uniform GaussBlurParam u_param;

void main()
{
	vec4 result = vec4(0.0f);
	for(int i = 0; i < u_param.sampleCount; i++)
	{
		// �K�E�X�̏d�݂���e�N�X�`���J���[�������s��
		result += u_param.offset[i].z * texture(u_blurSource, TexCoords + u_param.offset[i].xy);
	}
	result.w = 1.0f;
	out_fragColor = result;
}