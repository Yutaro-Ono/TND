//---------------------------------------------------------+
// スクリーン全面に指定されたテクスチャを貼る
//---------------------------------------------------------+
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_hdrTexture;
uniform sampler2D u_emissiveTexture;


void main()
{
	vec3 col = texture(u_hdrTexture, TexCoords).rgb;
	vec3 emissive = texture(u_hdrTexture, TexCoords).rgb;
	FragColor = vec4(col + emissive, 1.0);

	//FragColor = texture(u_screenTexture, TexCoords);
}