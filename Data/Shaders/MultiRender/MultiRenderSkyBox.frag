//----------------------------------------------------------+
// スカイボックス用フラグメントシェーダー
//----------------------------------------------------------+
#version 330 core
// 各バッファへの出力 (マルチレンダーターゲット)
layout (location = 0) out vec4 out_fragColor;
layout (location = 1) out vec4 out_brightColor;

in vec3 TexCoords;

uniform samplerCube u_skybox;

void main()
{
    // スカイボックス用テクスチャをサンプリング
	vec4 result = texture(u_skybox, TexCoords);
	// 通常カラーバッファへの出力
	out_fragColor = result;
	
	// 高輝度バッファへの出力値を抽出
	float brightness = dot(result.xyz, vec3(0.2126, 0.7152, 0.0722));     // 輝度をカラー結果の内積から求める
	
	if(brightness > 3.0)                                              // 輝度が0.5を超えたなら
	{
		out_brightColor = vec4(result.xyz, 0.0f);
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}


}