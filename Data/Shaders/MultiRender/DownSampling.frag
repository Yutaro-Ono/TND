//---------------------------------------------------------------+
// ダウンサンプリングシェーダ
// 入力:スクリーンシェーダ
//---------------------------------------------------------------+
#version 330 core
// 出力カラー
out vec4 out_fragColor;

in vec2 TexCoords;

uniform sampler2D u_screenTex;

void main()
{
	// スクリーン画像の縦横サイズをint型ベクトルとして保存
	ivec2 size = textureSize(u_screenTex, 0);

	vec2 texOffset = vec2(1.0f / size);

	vec3 c1 = vec3(texture(u_screenTex, TexCoords + texOffset * vec2(1.0f, 1.0f)));
	vec3 c2 = vec3(texture(u_screenTex, TexCoords + texOffset * vec2(-1.0f, 1.0f)));
	vec3 c3 = vec3(texture(u_screenTex, TexCoords + texOffset * vec2(1.0f, -1.0f)));
	vec3 c4 = vec3(texture(u_screenTex, TexCoords + texOffset * vec2(-1.0f, -1.0f)));
	vec3 c5 = vec3(texture(u_screenTex, TexCoords + texOffset * vec2(0.0f, 0.0f)));

	out_fragColor = vec4((c1 + c2 + c3 + c4 + c5) * 0.2, 1.0f);
}