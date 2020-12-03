//-----------------------------------------+
// ディレクショナルライトシェーダ
//-----------------------------------------+
#version 330 core
// 出力カラー
out vec4 out_fragColor;
// 頂点シェーダからの出力
in vec2 TexCoords;

// ディレクショナルライト構造体
struct DirectionalLight
{
	vec3 direction;
	vec3 ambientColor;
	vec3 color;
	vec3 specular;
	float intensity;
};

// GBuffer構造体
struct GBuffer
{
	sampler2D position;
	sampler2D normal;
	sampler2D albedoSpec;
};


// uniform
uniform DirectionalLight u_dirLight;
uniform GBuffer u_gBuffer;

uniform vec3 u_viewPos;      // カメラ位置

void main()
{
	// Gbufferの各要素をサンプリングして色情報を取り出す
	vec3 Position = texture(u_gBuffer.position, TexCoords).xyz;
	vec3 Normal = texture(u_gBuffer.normal, TexCoords).xyz;
	// アルベドとスペキュラを分けて保存
	vec4 AlbedoSpec = texture(u_gBuffer.albedoSpec, TexCoords);
	vec3 Albedo = AlbedoSpec.rgb;
	float Spec_p = AlbedoSpec.a;

	// アンビエント
	vec3 ambient =  u_dirLight.ambientColor * Albedo;
	vec3 LightDir = normalize(-u_dirLight.direction);
	Normal = normalize(Normal);
	float diff = max(dot(Normal, LightDir), 0.0f);

	// ディフューズ
	vec3 diffuse = u_dirLight.color * u_dirLight.intensity * Albedo * diff;

	// スペキュラ
	vec3 reflectDir = reflect(LightDir, Normal);
	vec3 viewDir = normalize(u_viewPos - Position);
	vec3 halfVec = normalize(LightDir + viewDir);
	float spec = pow(max(dot(Normal, halfVec), 0.0), 32);
	vec3 specular = u_dirLight.specular * u_dirLight.intensity * spec * Spec_p;

	vec3 result = ambient + diffuse + specular;
	out_fragColor = vec4(result, 1.0f);
}