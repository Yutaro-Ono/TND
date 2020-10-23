//-------------------------------------------------------+
// スプライト(2D)用フラグメントシェーダ
//-------------------------------------------------------+
#version 330 core

// 頂点シェーダからの入力
in vec2 FragTexCoords;           // テクスチャ座標

// カラーバッファへの出力
out vec4 outColor;

// uniform
uniform sampler2D u_Texture;     // テクスチャサンプリング

void main()
{
	// テクスチャからカラーをサンプリング
	outColor = texture(u_Texture, FragTexCoords);
}