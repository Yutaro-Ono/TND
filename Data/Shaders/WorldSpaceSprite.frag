//-----------------------------------------------------------------------+
// ワールド空間に配置するUI用フラグメントシェーダ
//-----------------------------------------------------------------------+
#version 330 core
// 出力カラー
out vec4 out_fragColor;

// 頂点シェーダからの入力 (テクスチャ座標)
in vec2 TexCoords;


// テクスチャサンプリング用
uniform sampler2D u_texture;

void main()
{
	vec4 color = texture(u_texture, TexCoords);
	out_fragColor = color;
}