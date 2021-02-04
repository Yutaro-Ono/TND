//----------------------------------------------------------------------------------------+
// HDR�ŏI�u�����h
// �W���J���[�o�b�t�@��Bloom���ʂ�K�p�����o�b�t�@���������ďo�͂���
//----------------------------------------------------------------------------------------+
#version 330 core
// �J���[�o��
out vec4 out_fragColor;

// ���_�V�F�[�_����̓���
in vec2 TexCoords;

uniform sampler2D u_scene;      // �V�[���̃e�N�X�`�� (�}���`�����_�[�^�[�Q�b�g��1�ԖځF�W���J���[�o�b�t�@)

// �k�����K�E�X�ڂ�����K�p����Bloom�e�N�X�`��
uniform sampler2D u_bloom1;
uniform sampler2D u_bloom2;
uniform sampler2D u_bloom3;
uniform sampler2D u_bloom4;
uniform sampler2D u_bloom5;

uniform float u_exposure;        // �I�o�l (0�ŉ�ʂ��^���ÂɂȂ�)
uniform float u_gamma;           // �K���}�R���N�V����

void main()
{
	vec3 hdrColor    = texture(u_scene, TexCoords).rgb;
    vec3 bloomColor1 = texture(u_bloom1, TexCoords).rgb * 1.0f;
    vec3 bloomColor2 = texture(u_bloom2, TexCoords).rgb * 1.5f;
    vec3 bloomColor3 = texture(u_bloom3, TexCoords).rgb * 2.0f;
    vec3 bloomColor4 = texture(u_bloom4, TexCoords).rgb * 3.0f;
    vec3 bloomColor5 = texture(u_bloom4, TexCoords).rgb * 9.0f;
    hdrColor += bloomColor1 + bloomColor2 + bloomColor3 + bloomColor4 + bloomColor5; 
  
    // �I�o�g�[���}�b�s���O
    vec3 mapped = vec3(1.0) - exp(-hdrColor * u_exposure);
    
    // �K���}�R���N�V����
    //const float gamma = 0.035;
    const float gamma = 0.085;

    mapped = pow(mapped, vec3(1.0 / u_gamma));  
    out_fragColor = vec4(mapped, 1.0);
}