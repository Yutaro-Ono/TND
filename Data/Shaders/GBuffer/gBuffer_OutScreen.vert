#version 330 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_texCoords;

out vec2 TexCoords;

void main()
{
	// スクリーン座標なのでz値はなし
	gl_Position = vec4(a_pos.x, a_pos.y, 0.0f, 1.0f);

	TexCoords = a_texCoords;
}