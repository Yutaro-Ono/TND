//---------------------------------------------------+
// ポイントライトシェーダ
//---------------------------------------------------+
#version 330 core
// 出力カラー
out vec4 FragColor;
// 頂点シェーダからの入力
in vec2 TexCoords;

// ポイントライト構造体
struct PointLight
{
	vec3 position;     // 光源座標
	vec3 ambient;      // ライトアンビエント
	vec3 diffuse;      // ライトディフューズ
	vec3 specular;     // ライトスペキュラ

	float constant;    // 定数
	float linear;      // 線形項
	float quadratic;   // 2乗項
	float radius;
};

// GBuffer構造体
struct GBuffer
{
	sampler2D position;
	sampler2D normal;
	sampler2D albedoSpec;
};

// uniform
uniform PointLight u_pl;
uniform GBuffer u_gBuffer;


uniform vec3 u_viewPos;            // カメラ座標

void main()
{
	// gBufferから各要素を得る
	vec3  Position   = texture(u_gBuffer.position, TexCoords).xyz;
	vec3  Normal     = texture(u_gBuffer.normal, TexCoords).xyz;
	vec4  albedoSpec = texture(u_gBuffer.albedoSpec, TexCoords);
	vec3  Albedo     = albedoSpec.rgb;
	float Spec_p     = albedoSpec.a;

	// 距離
	float l_distance = length(u_pl.position - Position);
	// 減衰率の算出
	float attenuation = u_pl.radius / (u_pl.constant + u_pl.linear * l_distance + u_pl.quadratic * (l_distance * l_distance));

	// ディフューズ
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_pl.position - Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_pl.diffuse * diff * Albedo;

	// スペキュラ
	vec3 viewDir = normalize(u_viewPos - Position);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfDir), 0.0), 32);
	vec3 specular = u_pl.specular * spec * Spec_p;

	// アンビエント
	vec3 ambient = u_pl.ambient * Albedo;

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);

}