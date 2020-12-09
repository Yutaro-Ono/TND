// ----------------------------------------------------------------
//  パーティクル用フラグメントシェーダー (Bloom対応)
// ----------------------------------------------------------------
// GLSL 3.3 を要求
#version 330 core
// 出力カラー
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_brightColor;

// 頂点シェーダからの入力
in VS_OUT
{
	vec2 fragTexCoords;          // テクスチャ座標
	vec3 fragNormal;             // ワールドスペース上の法線
	vec3 fragWorldPos;           // ワールドスペース上の座標
}fs_in;


// テクスチャサンプリング
uniform sampler2D uTexture;
uniform float     uAlpha;
uniform vec3      uColor;

void main()
{
    vec4 col = texture(uTexture, fs_in.fragTexCoords);
	out_color.rgb = col.rgb * uColor;
	out_color.a = col.a * uAlpha;

	// 高輝度バッファへの出力値を抽出
	float brightness = dot(out_color.rgb, vec3(0.0231, 0.11, 0.51));

	if(brightness > 0.3f)                                              // 輝度が0.4を超えたなら
	{
		out_brightColor = out_color;
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}
