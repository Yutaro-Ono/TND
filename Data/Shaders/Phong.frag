#version 330 core
// 出力カラー（出力ピクセルカラー）
out vec4 out_fragColor;

// 頂点シェーダーからの入力
// フラグメントへの出力
in VS_OUT
{
	vec2 fragTexCoords;          // テクスチャ座標
	vec3 fragNormal;             // ワールドスペース上の法線
	vec3 fragWorldPos;           // ワールドスペース上の座標
}fs_in;


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

// テクスチャサンプリング
uniform sampler2D u_texture;
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
	vec3 N = normalize(fs_in.fragNormal);
	// ポリゴン表面からライト方向へのベクトル
	vec3 L = normalize(-uDirLight.mDirection);
	// ポリゴン表面からカメラ方向
	vec3 V = normalize(u_viewPos - fs_in.fragWorldPos);
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
	out_fragColor = texture(u_mat.diffuseMap, fs_in.fragTexCoords) * vec4((Diffuse + u_ambientLight),1.0f) + vec4(Specular,1.0f);
}
