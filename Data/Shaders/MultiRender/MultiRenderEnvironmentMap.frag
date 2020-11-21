//-----------------------------------------------------+
// 環境マッピング (マルチレンダー対応)
//-----------------------------------------------------+
#version 330 core
layout (location = 0) out vec4 out_fragColor;
layout (location = 1) out vec4 out_brightColor;


// 頂点シェーダからの入力
in VS_OUT
{
	vec3 normal;
	vec3 fragWorldPos;
}fs_in;

uniform vec3 u_viewPos;
uniform samplerCube u_skybox;     // キューブマップ(スカイボックス)

void main()
{
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(fs_in.fragWorldPos - u_viewPos);        // カメラの向きベクトル
	vec3 R = refract(I, normalize(fs_in.normal), ratio);       // カメラの向きベクトルと法線から反射ベクトルを生成

	vec3 result = texture(u_skybox, R).rgb;

		// 高輝度バッファへの出力値を抽出
	float brightness = dot(result, vec3(0.1126, 0.4152, 0.522));     // 輝度をカラー結果の内積から求める

	if(brightness > 0.45)                                              // 輝度が0.5を超えたなら
	{
		out_brightColor = vec4(result, 0.0f);
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	// 通常カラーバッファへの出力
	out_fragColor = vec4(result, 0.8);
}