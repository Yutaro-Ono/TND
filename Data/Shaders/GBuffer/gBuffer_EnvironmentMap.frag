//-----------------------------------------------------+
// 環境マッピング (マルチレンダー対応)
//-----------------------------------------------------+
#version 330 core
// 各バッファへの出力 (GBuffer)
layout (location = 0) out vec3 out_gPos;
layout (location = 1) out vec3 out_gNormal;
layout (location = 2) out vec4 out_gAlbedoSpec;
layout (location = 3) out vec4 out_gBrightColor;

// 頂点シェーダからの入力
in VS_OUT
{
	vec3 fragNormal;
	vec3 fragWorldPos;
}fs_in;

uniform vec3 u_viewPos;
uniform samplerCube u_skybox;     // キューブマップ(スカイボックス)

uniform float u_luminance = 0.5f;

void main()
{
	float ratio = 1.00 / 1.52;      // 反射率
	vec3 I = normalize(fs_in.fragWorldPos - u_viewPos);        // カメラの向きベクトル
	vec3 R = refract(I, normalize(fs_in.fragNormal), ratio);       // カメラの向きベクトルと法線から反射ベクトルを生成
	// 透過度
	float alpha = 1.0f;

	// 出力カラー
	vec4 resultColor = vec4(texture(u_skybox, R).rgb, alpha);

	// GBuffer出力
	out_gPos = fs_in.fragWorldPos;
	out_gNormal = fs_in.fragNormal;
	out_gAlbedoSpec.rgb = resultColor.rgb;
	out_gAlbedoSpec.a = alpha;
	out_gBrightColor = resultColor * u_luminance;
}