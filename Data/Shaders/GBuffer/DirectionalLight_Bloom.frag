//-----------------------------------------+
// ディレクショナルライトシェーダ
//-----------------------------------------+
#version 330 core
// 出力
layout (location = 0) out vec4 out_colorBuffer;
layout (location = 1) out vec4 out_brightColor;
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
	sampler2D emissive;
};


// uniform
uniform DirectionalLight u_dirLight;
uniform GBuffer u_gBuffer;

uniform vec3 u_viewPos;                  // カメラ位置
uniform float u_brightLine = 1.2f;       // 高輝度判定となる基準

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
	vec3 viewDir = normalize(u_viewPos - Position);
	vec3 halfVec = normalize(LightDir + viewDir);
	float spec = pow(max(dot(Normal, halfVec), 0.0), 32);
	vec3 specular = u_dirLight.specular * spec * Spec_p;

	vec3 result = ambient + diffuse + specular + texture(u_gBuffer.emissive, TexCoords).rgb;
	vec3 brightColor = ambient + diffuse + texture(u_gBuffer.emissive, TexCoords).rgb;

	// 高輝度バッファへの出力値を抽出
	//float brightness = dot(result, vec3(0.1126, 0.4152, 0.522));     // 輝度をカラー結果の内積から求める
	float brightness = dot(result, vec3(0.1326, 0.1352, 0.642));

	if(brightness > u_brightLine)                                              // 輝度が0.8を超えたなら
	{
		out_brightColor = vec4(brightColor, 0.0f);
	}
	else
	{
		out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	out_colorBuffer = vec4(result, 1.0f);
}