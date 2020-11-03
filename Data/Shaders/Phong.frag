#version 330 core

// 頂点シェーダーからの入力
// テクスチャ座標
in vec2 fragTexCoord;
// 法線（ワールド空間）
in vec3 fragNormal;
// 頂点位置（ワールド空間）
in vec3 fragWorldPos;

// 出力カラー（出力ピクセルカラー）
out vec4 out_fragColor;

// テクスチャサンプリング
uniform sampler2D u_texture;
// テクスチャサンプリング用構造体
struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	sampler2D depthMap;
};
uniform Material u_mat;

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

// ライティング用変数
// カメラ位置（ワールド空間）
uniform vec3 u_viewPos;
// ポリゴン表面のスペキュラー強度
uniform float u_specPower;
// アンビエントライト色
uniform vec3 u_ambientLight;

// ディレクショナルライト
uniform DirectionalLight uDirLight;

void main()
{
	// ポリゴン表面の法線（フラグメントシェーダー上で補間されている）
	vec3 N = normalize(fragNormal);
	// ポリゴン表面からライト方向へのベクトル
	vec3 L = normalize(-uDirLight.mDirection);
	// ポリゴン表面からカメラ方向
	vec3 V = normalize(u_viewPos - fragWorldPos);
	// -L ベクトルを 法線 N に対して反射したベクトルRを求める
	vec3 R = normalize(reflect(-L, N));

	// フォン反射計算
	vec3 Phong = u_ambientLight;
	float NdotL = dot(N, L);

	vec3 Diffuse;
	vec3 Specular;

	Diffuse = uDirLight.mDiffuseColor * max(NdotL,0.0f);
	Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), u_specPower);

	// 最終カラーを出力 (alpha = 1)
	out_fragColor = texture(u_mat.diffuseMap, fragTexCoord) * vec4((Diffuse + u_ambientLight),1.0f) + vec4(Specular,1.0f);
}
