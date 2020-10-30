//----------------------------------------------------+
// スキン入りメッシュ用頂点配列
//----------------------------------------------------+
#version 330 core
// Attribute
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in uvec4 inSkinBones;
layout(location = 3) in vec4 inSkinWeights;
layout(location = 4) in vec2 inTexCoord;

// 変換行列 (ワールド、ビュー&プロジェクション)
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
// 行列パレット
uniform mat4 uMatrixPalette[196];
// ライト空間行列
uniform mat4 uLightSpaceMatrix;

// 頂点への出力
out vec2 fragTexCoord;        // テクスチャ座標
out vec3 fragNormal;          // ワールドスペース上の法線
out vec3 fragWorldPos;        // ワールドスペース上の頂点座標
out vec4 fragPosLightSpace;      // ライトスペース上の座標

void main()
{
	// 頂点座標を四次元に変換
	vec4 pos = vec4(inPosition, 1.0);
	
	// スキン位置を算出
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// スキン位置をワールド座標変換
	skinnedPos = skinnedPos * uWorldTransform;
	// ワールド上のスキン位置を保存
	fragWorldPos = skinnedPos.xyz;
	// クリップ空間へ頂点を変換し出力に渡す
	gl_Position = skinnedPos * uViewProj;

	// スキン位置の法線を算出
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x
		+ (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y
		+ (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z
		+ (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;
	// 法線をワールド空間上に変換(w = 0)
	fragNormal = (skinnedNormal * uWorldTransform).xyz;

	// テクスチャ座標を出力
	fragTexCoord = inTexCoord;

	fragPosLightSpace = vec4(fragWorldPos, 1.0) * uLightSpaceMatrix;

}

