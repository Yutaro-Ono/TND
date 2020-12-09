//-------------------------------------------------------+
// スプライト(2D)用フラグメントシェーダ
//-------------------------------------------------------+
#version 330 core
// 出力カラー
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;

// 頂点シェーダからの入力
in vec2 FragTexCoords;           // テクスチャ座標


// uniform
uniform sampler2D u_Texture;     // テクスチャサンプリング

void main()
{

	// テクスチャからカラーをサンプリング
	out_color = texture(u_Texture, FragTexCoords);

	vec3 result = out_color.rgb;

	// 高輝度バッファへの出力値を抽出
	float brightness = dot(result, vec3(0.0231, 0.11, 0.51));

	if(brightness > 0.31)                                              // 輝度が0.4を超えたなら
	{
		out_brightColor = out_color * 0.6f;
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

}