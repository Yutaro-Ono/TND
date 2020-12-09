// ----------------------------------------------------------------
//  パーティクル用フラグメントシェーダー
// ----------------------------------------------------------------

// GLSL 3.3 を要求
#version 330 core

// 頂点シェーダからの入力
in VS_OUT
{
	vec2 fragTexCoords;          // テクスチャ座標
	vec3 fragNormal;             // ワールドスペース上の法線
	vec3 fragWorldPos;           // ワールドスペース上の座標
}fs_in;

// 出力カラー（出力ピクセルカラー）
out vec4 outColor;

// テクスチャサンプリング
uniform sampler2D uTexture;
uniform float     uAlpha;
uniform vec3      uColor;

void main()
{
    vec4 col = texture(uTexture, fs_in.fragTexCoords);
	outColor.rgb = col.rgb * uColor;
	outColor.a = col.a * uAlpha;
}
