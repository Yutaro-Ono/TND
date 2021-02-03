//-----------------------------------------------------------------------+
// ワールド空間に配置するUI用頂点シェーダ
//-----------------------------------------------------------------------+
#version 420
// attribute
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

// uniformバッファブロック
// 0.行列
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};


// 各変換行列
uniform mat4 u_worldTransform;

// フラグメントへの出力 (テクスチャ座標)
out vec2 TexCoords;

void main()
{
	// 頂点座標をgl_Position用にvec4型へ変換
	vec4 pos = vec4(a_pos, 1.0);

	mat4 world = u_worldTransform;
	mat4 proj = u_projection;
	mat4 view = u_view;

	pos = pos * world * view * proj;

	// クリップ空間にワールド座標を変換
	gl_Position = pos;
	
	// テクスチャ座標を出力
	TexCoords = a_texCoords;

}