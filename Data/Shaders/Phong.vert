#version 330 core

// attribute
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// ワールド変換行列・ビュープロジェクション行列
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;


// フラグメントへの出力
// テクスチャ座標
out vec2 fragTexCoord;
// ワールドスペース上の法線
out vec3 fragNormal;
// ワールドスペース上の座標
out vec3 fragWorldPos;

void main()
{
	// vec4型に頂点座標を変換
	vec4 pos = vec4(inPosition, 1.0);
	// 頂点情報をワールドスペースに変換
	pos = pos * uWorldTransform;
	// フラグメント出力にセット
	fragWorldPos = pos.xyz;
	// クリップ空間にワールド上の頂点座標を変換
	gl_Position = pos * uViewProj;

	// 法線情報をワールドスペースに変換 (w = 0)
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// テクスチャ座標をフラグメントシェーダへ出力
	fragTexCoord = inTexCoord;
}
