//----------------------------------------------------------+
// �X�J�C�{�b�N�X�p�t���O�����g�V�F�[�_�[
//----------------------------------------------------------+
#version 330 core
// �e�o�b�t�@�ւ̏o�� (GBuffer)
layout (location = 0) out vec3 out_gPos;
layout (location = 1) out vec3 out_gNormal;
layout (location = 2) out vec4 out_gAlbedoSpec;

// ���_����̓���
in VS_OUT
{
	vec3 fragTexCoords;              // �e�N�X�`�����W
	vec3 fragNormal;                 // �@���i���[���h��ԁj
	vec3 fragWorldPos;               // ���_�ʒu�i���[���h��ԁj
}fs_in;

uniform samplerCube skybox;

void main()
{
    // �A���x�h�o��
	out_gAlbedoSpec = texture(skybox, fs_in.fragTexCoords);
}