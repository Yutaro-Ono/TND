//-----------------------------------------+
// ポイントライトシェーダ
//-----------------------------------------+
#version 330 core
// attribute (頂点座標のみで良い)
layout (location = 0) in vec3 a_pos;
// uniform
uniform mat4 u_worldTransform;
uniform mat4 u_view;
uniform mat4 u_projection;

// フラグメントへの出力
out vec2 TexCoords;

void main()
{
	// 座標変換
	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform * u_view * u_projection;
	gl_Position = pos;

	// 正規化デバイス座標→gBufferのテクスチャ座標に変換
	// posのx, yをwで除算し、テクスチャ座標の[0.0 〜 1.0]へ変換
	TexCoords.x = pos.x / pos.w * 0.5f + 0.5f;
	TexCoords.y = pos.y / pos.w * 0.5f + 0.5f;

}