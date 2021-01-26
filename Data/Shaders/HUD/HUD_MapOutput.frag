//------------------------------------------+
// マップHUDのスクリーン出力
//------------------------------------------+
#version 330 core
// 出力カラー
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;
in vec2 fragTexCoord;

// uniform
uniform sampler2D u_texture;     // テクスチャサンプリング
uniform float u_intensity = 1.0f;  // 輝度調節

// 切り抜き用円形テクスチャ
uniform sampler2D u_mapTex;

void main()
{
	
	// マップテクスチャのサンプリング
	// このテクスチャの黒色部分のみに描画を適用していく
	vec4 mapTexture = texture(u_mapTex, fragTexCoord);

	// マップを描画した一時保存テクスチャのカラーサンプリング
	vec3 mapColor = texture(u_texture, fragTexCoord).rgb;

	//out_color = texture(u_texture, fragTexCoord);

	vec3 color = vec3(1.0, 1.0, 1.0);
	// マップテクスチャの領域(黒丸)内のみカラー出力
	if(mapTexture.a == 1.0)
	{
		// 描画結果を出力
		out_color = vec4(mapColor, 0.925);

		// 何も描画されていない場合 (ノーカラー)
		// 黒で塗りつぶし
		if(out_color.a == 0.0)
		{
			out_color = vec4(0.1, 0.1, 1.0, 0.9);
		}
	}
	else
	{
		out_color = vec4(0.0, 0.0, 0.0, 0.0);
	}

	// マップの枠線描画
	// 色値が一定以上 (白に近い) 場合。少々無理やりなので改善したい
	if(mapTexture.r > 0.8f && mapTexture.g > 0.8f && mapTexture.b > 0.8f)
	{
		out_color = mapTexture;
	}

	// 高輝度バッファは使用しない
	out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

}
