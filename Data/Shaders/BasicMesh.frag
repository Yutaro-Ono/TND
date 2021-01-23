#version 330 core
out vec4 outColor;

in vec2 fragTexCoord;


uniform sampler2D u_texture;

void main()
{
    outColor = texture(u_texture, fragTexCoord);
}
