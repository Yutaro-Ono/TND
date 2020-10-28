#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uLightSpaceMatrix;    // ���C�g�X�y�[�X�ϊ��s��
uniform mat4 uWorldTransform;

void main()
{
    //gl_Position = uLightSpaceMatrix * uWorldTransform * vec4(aPos, 1.0);
    gl_Position = vec4(aPos, 1.0) * uWorldTransform * uLightSpaceMatrix;
}  