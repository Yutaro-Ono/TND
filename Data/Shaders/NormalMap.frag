//---------------------------------------------------------------------+
// フラグメントシェーダ
// 3Dモデル(メッシュ)描画用・法線マップ対応
//---------------------------------------------------------------------+
#version 330 core

out vec4 out_fragColor;       // 出力先：カラー

uniform vec3 uCameraPos;

// 頂点シェーダーからの入力受け取り
in VS_OUT
{
	vec2 TexCoords;
	vec3 FragNormal;
	vec3 FragPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;

}fs_in;

// マテリアル情報構造体
struct Material
{
	sampler2D diffuseMap;     // ディフューズマップ
	sampler2D specularMap;    // スペキュラマップ
	sampler2D normalMap;      // 法線マップ
	float     shininess;      // 反射光の強さ
};

// ライト(光源)情報構造体
struct Light
{
	vec3 mPosition;
	vec3 mDirection;
	float cutoff;
	float outercutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 mSpecColor;

	float constant;
	float linear;
	float quadratic;
};

// ディレクショナルライト用構造体
struct DirectionalLight
{
	// ライト方向
	vec3 mDirection;
	// ディフューズ色
	vec3 mDiffuseColor;
	// スペキュラー色
	vec3 mSpecColor;
};

uniform Material u_mat;
uniform Light u_light;
// ディレクショナルライト
uniform DirectionalLight uDirLight;

void main()
{
	// 範囲[0, 19の法線マップベクトルを取得
	vec3 normal = texture(u_mat.normalMap, fs_in.TexCoords).rgb;
	// 法線ベクトルの範囲を[-1, +1]の範囲に復元する(タンジェントスペースに変換)
	normal = normalize(normal * 2.0 - 1.0);


	// diffuseColor
	vec3 color = texture(u_mat.diffuseMap, fs_in.TexCoords).rgb;
	// ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    //vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
	//vec3 diffuse =uDirLight.mDiffuseColor * max(dot(uDirLight.mDirection, normal), 0.0) * color;

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
		// ポリゴン表面の法線（フラグメントシェーダー上で補間されている）
	vec3 N = normalize(fs_in.FragNormal);
	// ポリゴン表面からライト方向へのベクトル
	vec3 L = normalize(-uDirLight.mDirection);
	// ポリゴン表面からカメラ方向
	vec3 V = normalize(uCameraPos - fs_in.FragPos);
	// -L ベクトルを 法線 N に対して反射したベクトルRを求める
	vec3 R = normalize(reflect(-L, N));
	// フォン反射計算
	vec3 Phong = ambient;
	float NdotL = dot(N, L);


	vec3 Diffuse = diff * color * max(NdotL, 0.0f);
    vec3 specular = vec3(0.2) * spec;
	// フラグメントカラーを出力
    out_fragColor = vec4((Diffuse + ambient), 1.0) + vec4(specular, 1.0f);
}