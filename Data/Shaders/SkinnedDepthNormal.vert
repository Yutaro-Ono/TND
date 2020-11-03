//----------------------------------------------------+
// スキン入りメッシュ用頂点配列
//----------------------------------------------------+
#version 330 core
// Attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in uvec4 a_skinBones;
layout(location = 3) in vec4 a_skinWeights;
layout(location = 4) in vec2 a_texCoords;
layout(location = 5) in vec3 a_tangent;

// 変換行列 (ワールド、ビュー&プロジェクション)
uniform mat4 u_worldTransform;
// 行列パレット
uniform mat4 u_matrixPalette[196];
// ライト空間行列
uniform mat4 u_lightSpaceMatrix;


out vec4 fragPosLightSpace;      // ライトスペース上の座標

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

	// クリップ空間へ頂点を変換し出力に渡す
	gl_Position = skinnedPos * u_lightSpaceMatrix;


}

