//-----------------------------------------+
// ディレクショナルライトシェーダ
//-----------------------------------------+
#version 330 core
// attribute (頂点座標のみで良い)
layout (location = 0) in vec2 a_pos;

out vec2 TexCoords;

void main()
{

	vec4 pos = vec4(a_pos, 0.0, 1.0);

	gl_Position = pos;

	// 正規化デバイス座標→gBufferのテクスチャ座標に変換
	// posのx, yをwで除算し、テクスチャ座標の[0.0 ～ 1.0]へ変換
	TexCoords.x = pos.x / pos.w * 0.5f + 0.5f;
	TexCoords.y = pos.y / pos.w * 0.5f + 0.5f;
}