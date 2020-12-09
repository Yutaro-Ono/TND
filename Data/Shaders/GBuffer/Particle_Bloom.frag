// ----------------------------------------------------------------
//  �p�[�e�B�N���p�t���O�����g�V�F�[�_�[ (Bloom�Ή�)
// ----------------------------------------------------------------
// GLSL 3.3 ��v��
#version 330 core
// �o�̓J���[
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;

// ���_�V�F�[�_����̓���
in VS_OUT
{
	vec2 fragTexCoords;          // �e�N�X�`�����W
	vec3 fragNormal;             // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;           // ���[���h�X�y�[�X��̍��W
}fs_in;


// �e�N�X�`���T���v�����O
uniform sampler2D uTexture;
uniform float     uAlpha;
uniform vec3      uColor;

void main()
{
    vec4 col = texture(uTexture, fs_in.fragTexCoords);
	out_color.rgb = col.rgb * uColor;
	out_color.a = col.a * uAlpha;

	// ���P�x�o�b�t�@�ւ̏o�͒l�𒊏o
	float brightness = dot(out_color.rgb, vec3(0.0231, 0.11, 0.51));

	if(brightness > 0.3f)                                              // �P�x��0.4�𒴂����Ȃ�
	{
		out_brightColor = out_color;
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}
