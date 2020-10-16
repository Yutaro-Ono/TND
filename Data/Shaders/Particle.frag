// ----------------------------------------------------------------
//  パーティクル用フラグメントシェーダー
// ----------------------------------------------------------------

// GLSL 3.3 を要求
#version 330 core

// 頂点シェーダーからの入力
// テクスチャ座標
in vec2 fragTexCoord;
// 法線（ワールド空間）
in vec3 fragNormal;
// 頂点位置（ワールド空間）
in vec3 fragWorldPos;


// 出力カラー（出力ピクセルカラー）
out vec4 outColor;

// テクスチャサンプリング
uniform sampler2D uTexture;
uniform float     uAlpha;
uniform vec3      uColor;

void main()
{
    vec4 col = texture(uTexture, fragTexCoord);
	outColor.rgb = col.rgb * uColor;
	outColor.a = col.a * uAlpha;
}
