//-------------------------------------------------------+
// スプライト(2D)用頂点シェーダ
//-------------------------------------------------------+
#version 330 core

// アトリビュート (バッファのAttribPointerで指定したデータ)
layout (location = 0) in vec3 a_Pos;              // 頂点座標
layout (location = 1) in vec3 a_Normal;           // 法線
layout (location = 2) in vec2 a_TexCoords;         // テクスチャ座標(UV)

// フラグメントシェーダへの出力
out vec2 FragTexCoords;

// Uniform指定子
uniform mat4 u_worldTransform;     // ワールド変換行列
uniform mat4 u_viewProj;           // ビュー * プロジェクションの合成行列

void main()
{
	// 頂点座標をgl_Position用にvec4型へ変換
	vec4 pos = vec4(a_Pos, 1.0);
	// クリップ空間にワールド座標を変換
	gl_Position = pos * u_worldTransform * u_viewProj;

	// テクスチャ座標をフラグメントへ出力
	FragTexCoords = a_TexCoords;
}