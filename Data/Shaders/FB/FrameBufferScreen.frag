//---------------------------------------------------------+
// �X�N���[���S�ʂɎw�肳�ꂽ�e�N�X�`����\��
//---------------------------------------------------------+
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_screenTexture;

void main()
{
	vec3 col = texture(u_screenTexture, TexCoords).rgb;
	FragColor = vec4(col, 1.0);

	//FragColor = texture(u_screenTexture, TexCoords);
}