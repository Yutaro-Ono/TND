//---------------------------------------------------+
// スポットライト・フラグメントシェーダ
// 出力:標準カラーバッファ・高輝度バッファ
//---------------------------------------------------+
#version 330 core
// 出力
layout (location = 0) out vec4 out_colorBuffer;
layout (location = 1) out vec4 out_brightColor;
// 頂点シェーダからの入力
in vec2 TexCoords;

// スポットライト構造体
struct SpotLight
{
	vec3 position;     // 光源座標
	vec3 direction;    // 方向
	vec3 ambient;      // ライトアンビエント
	vec3 diffuse;      // ライトディフューズ
	vec3 specular;     // ライトスペキュラ

	float cutOff;      // カットオフ
	float outerCutOff;

	float constant;    // 定数
	float linear;      // 線形項
	float quadratic;   // 2乗項

	float luminance;   // 輝度
};

// GBuffer構造体
struct GBuffer
{
	sampler2D pos;
	sampler2D normal;
	sampler2D albedoSpec;
	sampler2D emissive;
};

// uniform
uniform SpotLight u_spotL;
uniform GBuffer u_gBuffer;

uniform vec3 u_viewPos;            // カメラ座標

void main()
{
	// gBufferから各要素を得る
	vec3  Position   = texture(u_gBuffer.pos, TexCoords).xyz;
	vec3  Normal     = texture(u_gBuffer.normal, TexCoords).xyz;
	vec4  albedoSpec = texture(u_gBuffer.albedoSpec, TexCoords);
	vec3  Albedo     = albedoSpec.rgb;
	float Spec_p     = albedoSpec.a;

	vec3 lightDir = normalize(u_spotL.position - Position);
	float theta = dot(lightDir, normalize(-u_spotL.direction));



	if(theta > u_spotL.cutOff)
	{
		// アンビエント
		vec3 ambient = u_spotL.ambient * Albedo;
		// ディフューズ
		vec3 norm = normalize(Normal);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = u_spotL.diffuse * diff * Albedo * u_spotL.luminance;

		// スペキュラ
		vec3 viewDir = normalize(u_viewPos - Position);
		vec3 halfDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfDir), 0.0), 128);
		vec3 specular = u_spotL.specular * spec * Spec_p;


		// 距離
		float l_distance = length(u_spotL.position - Position);
		// 減衰率の算出
		float attenuation = 1.0f / (u_spotL.constant + u_spotL.linear * l_distance + u_spotL.quadratic * (l_distance * l_distance));
		//ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular + texture(u_gBuffer.emissive, TexCoords).rgb;

		// 高輝度バッファへの出力値を抽出
		float brightness = dot(result, vec3(0.1326, 0.3352, 0.442));

		if(brightness > 0.2)                                              // 輝度が0.4を超えたなら
		{
			out_brightColor = vec4(result, Spec_p);
		}
		else
		{
			out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		out_colorBuffer = vec4(result, 1.0);
	}
	else
	{
		out_colorBuffer = vec4(u_spotL.ambient * Albedo, Spec_p);

		// 高輝度バッファへの出力値を抽出
		float brightness = dot(u_spotL.ambient * Albedo, vec3(0.1326, 0.3352, 0.442));

		if(brightness > 0.2)                                              // 輝度が0.4を超えたなら
		{
			out_brightColor = vec4(u_spotL.ambient * Albedo, Spec_p);
		}
		else
		{
			out_brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}







}