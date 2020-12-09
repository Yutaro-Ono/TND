//-----------------------------------------------------------------------+
// ワールド空間に配置するUI用フラグメントシェーダ
//-----------------------------------------------------------------------+
#version 330 core
// 出力カラー
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;
// 頂点シェーダからの入力 (テクスチャ座標)
in vec2 TexCoords;


// テクスチャサンプリング用
uniform sampler2D u_texture;

void main()
{
	vec4 color = texture(u_texture, TexCoords);
	out_color = color;

	// 高輝度バッファへの出力値を抽出
	float brightness = dot(out_color.rgb, vec3(0.0231, 0.11, 0.51));

	if(brightness > 0.3f)                                              // 輝度が0.4を超えたなら
	{
		out_brightColor = out_color * 0.3f;
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}