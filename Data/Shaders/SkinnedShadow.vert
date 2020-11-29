//----------------------------------------------------+
// スキンメッシュ頂点シェーダ (シャドウ用)
//----------------------------------------------------+
#version 330 core
// Attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in uvec4 a_skinBones;
layout(location = 3) in vec4 a_skinWeights;
layout(location = 4) in vec2 a_texCoords;

// 変換行列 (ワールド、ビュー、プロジェクション)
uniform mat4 u_worldTransform;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_viewProj;
// 行列パレット
uniform mat4 u_matrixPalette[196];
// ライト空間行列
uniform mat4 u_lightSpaceMatrix;

// フラグメントへの出力
out VS_OUT
{
	vec2 fragTexCoords;          // テクスチャ座標
	vec3 fragNormal;             // ワールドスペース上の法線
	vec3 fragWorldPos;           // ワールドスペース上の座標
	vec4 fragPosLightSpace;      // ライトスペース上の座標
}vs_out;

void main()
{
	// 頂点座標を四次元に変換
	vec4 pos = vec4(a_pos, 1.0);
	
	// スキン位置を算出
	vec4 skinnedPos = (pos * u_matrixPalette[a_skinBones.x]) * a_skinWeights.x;
	skinnedPos += (pos * u_matrixPalette[a_skinBones.y]) * a_skinWeights.y;
	skinnedPos += (pos * u_matrixPalette[a_skinBones.z]) * a_skinWeights.z;
	skinnedPos += (pos * u_matrixPalette[a_skinBones.w]) * a_skinWeights.w;

	// スキン位置をワールド座標変換
	skinnedPos = skinnedPos * u_worldTransform;
	// ワールド上のスキン位置を保存
	vs_out.fragWorldPos = skinnedPos.xyz;
	// クリップ空間へ頂点を変換し出力に渡す
	gl_Position = skinnedPos * u_view * u_projection;
	// スキン位置の法線を算出
	vec4 skinnedNormal = vec4(a_normal, 0.0f);
	skinnedNormal = (skinnedNormal * u_matrixPalette[a_skinBones.x]) * a_skinWeights.x
		+ (skinnedNormal * u_matrixPalette[a_skinBones.y]) * a_skinWeights.y
		+ (skinnedNormal * u_matrixPalette[a_skinBones.z]) * a_skinWeights.z
		+ (skinnedNormal * u_matrixPalette[a_skinBones.w]) * a_skinWeights.w;
	// 法線をワールド空間上に変換(w = 0)
	vs_out.fragNormal = (skinnedNormal * u_worldTransform).xyz;

	// テクスチャ座標を出力
	vs_out.fragTexCoords = a_texCoords;

	vs_out.fragPosLightSpace = vec4(vs_out.fragWorldPos, 1.0) * u_lightSpaceMatrix;

}

