//------------------------------------------+
// �}�b�v�`��p�̒P�F�o��
//------------------------------------------+
#version 330 core
out vec4 outColor;

in vec2 fragTexCoord;

// �w��F
uniform vec3 u_mapColor;

void main()
{
    outColor = vec4(u_mapColor, 1.0f);
}
