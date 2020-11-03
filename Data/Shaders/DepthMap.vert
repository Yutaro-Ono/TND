#version 330 core
layout (location = 0) in vec3 a_pos;

uniform mat4 u_lightSpaceMatrix;    // ���C�g�X�y�[�X�ϊ��s��
uniform mat4 u_worldTransform;

void main()
{

    gl_Position = vec4(a_pos, 1.0) * u_worldTransform * u_lightSpaceMatrix;
}  