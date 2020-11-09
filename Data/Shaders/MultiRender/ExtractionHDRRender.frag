//----------------------------------------------------------+
// マルチレンダリング用 色値・高輝度抽出シェーダ
//----------------------------------------------------------+
#version 330 core
// 各バッファへの出力
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

// 頂点シェーダからの入力
in VS_OUT
{

	vec2 fragTexCoords;

}fs_in;

void main()
{
	vec3 result = vec3(texture(

}