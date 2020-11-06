//-----------------------------------------------------+
// 環境マッピング
//-----------------------------------------------------+
#version 330 core
out vec4 out_fragColor;

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
	vec3 I = normalize(fs_in.fragWorldPos - u_viewPos);     // カメラの向きベクトル
	vec3 R = reflect(I, normalize(fs_in.normal));       // カメラの向きベクトルと法線から反射ベクトルを生成
	out_fragColor = vec4(texture(u_skybox, R).rgb, 1.0);    // 反射ベクトルを用いてスカイボックスをサンプリング
}