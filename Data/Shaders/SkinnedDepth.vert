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
// 行列パレット
uniform mat4 uMatrixPalette[196];
// ライト空間行列
uniform mat4 uLightSpaceMatrix;


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

	// クリップ空間へ頂点を変換し出力に渡す
	gl_Position = skinnedPos * uLightSpaceMatrix;


}

