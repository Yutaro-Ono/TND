//-----------------------------------------------------+
// ���}�b�s���O
//-----------------------------------------------------+
#version 330 core
out vec4 out_fragColor;

// ���_�V�F�[�_����̓���
in VS_OUT
{
	vec3 normal;
	vec3 fragWorldPos;
}fs_in;

uniform vec3 u_viewPos;
uniform samplerCube u_skybox;     // �L���[�u�}�b�v(�X�J�C�{�b�N�X)

void main()
{
	float ratio = 1.00 / 1.52;                                 // ���˗�
	vec3 I = normalize(fs_in.fragWorldPos - u_viewPos);        // �J�����̌����x�N�g��
	vec3 R = refract(I, normalize(fs_in.normal), ratio);       // �J�����̌����x�N�g���Ɩ@�����甽�˃x�N�g���𐶐�
	
	out_fragColor = vec4(texture(u_skybox, R).rgb, 1.0);       // ���˃x�N�g����p���ăX�J�C�{�b�N�X���T���v�����O
}