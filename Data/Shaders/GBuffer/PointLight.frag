//---------------------------------------------------------+
// ポイントライトシェーダ (フラグメント)
//---------------------------------------------------------+
#version 330 core
// 出力カラー
out vec4 out_fragColor;
// 頂点シェーダからの入力
in vec2 TexCoords;

// ポイントライト構造体
struct PointLight
{
	vec3 position;      // 光源座標
	
	vec3 diffuse;       // ライトディフューズ
	vec3 ambient;       // ライトアンビエント
	vec3 specular;      // ライトスペキュラ

	float constant;     // 定数
	float linear;       // 線形項
	float quadratic;    // 2乗項
};

// GBuffer構造体
struct GBuffer
{
	sampler2D gPos;
	sampler2D gNormal;
	sampler2D gAlbedoSpec;
};

// uniform
uniform PointLight u_pointLight;
uniform GBuffer u_gBuffer;

uniform vec3 u_viewPos;        // カメラ座標



void main()
{
	// GBufferの各要素をサンプリングして色情報を取り出す
	vec3 Position = texture(u_gBuffer.gPos, TexCoords).xyz;
	vec3 Normal = texture(u_gBuffer.gNormal, TexCoords).xyz;
	// アルベドとスペキュラを分けて保存
	vec4 AlbedoSpec = texture(u_gBuffer.gAlbedoSpec, TexCoords);
	vec3 Albedo = AlbedoSpec.rgb;
	float Spec_p = AlbedoSpec.a;

	// 頂点とポイントライトとの距離を計算
	float l_distance = length(u_pointLight.position - Position);
	float attenuation = 1.0 / (u_pointLight.constant + u_pointLight.linear * l_distance + u_pointLight.quadratic * (l_distance * l_distance));

	// ディフューズ
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_pointLight.position - Position);     // ライト方向の算出
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_pointLight.diffuse * diff * Albedo;

	// スペキュラ
	vec3 viewDir = normalize(u_viewPos - Position);     // カメラの向きを算出
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfDir), 0.0), 32);
	vec3 specular = u_pointLight.specular * spec * Spec_p;

	// アンビエント
	vec3 ambient = u_pointLight.ambient * Albedo;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// 最終出力
	vec3 result = ambient + diffuse + specular;
	out_fragColor = vec4(result, 1.0);

}