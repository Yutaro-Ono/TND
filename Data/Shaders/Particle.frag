// ----------------------------------------------------------------
//  �p�[�e�B�N���p�t���O�����g�V�F�[�_�[
// ----------------------------------------------------------------

// GLSL 3.3 ��v��
#version 330 core

// ���_�V�F�[�_����̓���
in VS_OUT
{
	vec2 fragTexCoords;          // �e�N�X�`�����W
	vec3 fragNormal;             // ���[���h�X�y�[�X��̖@��
	vec3 fragWorldPos;           // ���[���h�X�y�[�X��̍��W
}fs_in;

// �o�̓J���[�i�o�̓s�N�Z���J���[�j
out vec4 outColor;

// �e�N�X�`���T���v�����O
uniform sampler2D uTexture;
uniform float     uAlpha;
uniform vec3      uColor;

void main()
{
    vec4 col = texture(uTexture, fs_in.fragTexCoords);
	outColor.rgb = col.rgb * uColor;
	outColor.a = col.a * uAlpha;
}
