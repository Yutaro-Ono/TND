#version 330 core

// attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

// ワールド変換行列・ビュープロジェクション行列
uniform mat4 u_worldTransform;
uniform mat4 u_viewProj;


// フラグメントへの出力
out VS_OUT
{
	vec2 fragTexCoords;          // テクスチャ座標
	vec3 fragNormal;             // ワールドスペース上の法線
	vec3 fragWorldPos;           // ワールドスペース上の座標
}vs_out;

void main()
{
	// vec4型に頂点座標を変換
	vec4 pos = vec4(a_pos, 1.0);
	// 頂点情報をワールドスペースに変換
	pos = pos * u_worldTransform;
	// フラグメント出力にセット
	vs_out.fragWorldPos = pos.xyz;
	// クリップ空間にワールド上の頂点座標を変換
	gl_Position = pos * u_viewProj;

	// 法線情報をワールドスペースに変換 (w = 0)
	vs_out.fragNormal = (vec4(a_normal, 0.0f) * u_worldTransform).xyz;

	// テクスチャ座標をフラグメントシェーダへ出力
	vs_out.fragTexCoords = a_texCoords;
}
