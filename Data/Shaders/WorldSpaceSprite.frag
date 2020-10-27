//-----------------------------------------------------------------------+
// ワールド空間に配置するUI用フラグメントシェーダ
//-----------------------------------------------------------------------+
#version 330 core

// 頂点シェーダからの入力 (テクスチャ座標)
in vec2 FragTexCoords;

// 出力カラー
out vec4 out_FragColor;

// テクスチャサンプリング用
uniform sampler2D u_Texture;

void main()
{
	vec4 color = texture(u_Texture, FragTexCoords);
	out_FragColor = color;
	//out_FragColor.a = 0.1f;
}