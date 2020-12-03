#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;


uniform mat4 u_worldTransform;
uniform mat4 u_view;
uniform mat4 u_projection;

// ���_�V�F�[�_�[�ւ̏o��
out VS_OUT
{
	vec2 fragTexCoords;              // �e�N�X�`�����W
	vec3 fragNormal;                 // �@���i���[���h��ԁj
	vec3 fragWorldPos;               // ���_�ʒu�i���[���h��ԁj
}vs_out;

void main()
{
	// ���_���W�����[���h��Ԃɕϊ�
	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform;
	
	gl_Position = pos * u_view * u_projection;
	// �e��o�͂ɃZ�b�g
	vs_out.fragWorldPos = vec3(vec4(a_pos, 1.0) * u_worldTransform);
	vs_out.fragNormal = a_normal * mat3(transpose(inverse(u_worldTransform)));
	vs_out.fragTexCoords = a_texCoords;
}
