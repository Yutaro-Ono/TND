//-------------------------------------------------------------------------+
// GBuffer・車のモデル用 フラグメントシェーダ
// シャドウ・環境マップ適用
//-------------------------------------------------------------------------+
#version 330 core
// GBuffer出力
layout (location = 0) out vec3 out_gPosition;
layout (location = 1) out vec3 out_gNormal;
layout (location = 2) out vec4 out_gAlbedoSpec;
layout (location = 3) out vec4 out_gBrightColor;

// 頂点シェーダからの入力
in VS_OUT
{
	vec2 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                // ワールドスペース上の法線
	vec3 fragWorldPos;              // ワールドスペース上の座標
	vec3 fragViewPos;               // カメラ座標
	vec4 fragPosLightSpace;         // ライトスペース上の座標
}fs_in;

// マテリアル構造体
struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D depthMap;
	sampler2D emissiveMap;
};

// ディレクショナルライト用構造体
struct DirectionalLight
{
	vec3 direction;      // ライト方向
	vec3 ambient;        // アンビエント
	vec3 diffuse;        // ディフューズ色
	vec3 specular;       // スペキュラー色
};

// 各種構造体 (uniform)
uniform Material u_mat;
uniform DirectionalLight u_dirLight;

uniform samplerCube u_skybox;     // サンプリング用キューブマップ

uniform float u_specPower;

// シャドウの計算
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // パースペクティブ除算(遠近感を出すための計算だが、今回はw成分が1のため意味なし)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 正規化デバイス座標をテクスチャ座標と合わせるため、[-1～1]を[0～1]の範囲に変換
    projCoords = projCoords * 0.5 + 0.5;
    // シャドウマップよりライトに最も近いフラグメントの深度値を得る
    float closestDepth = texture(u_mat.depthMap, projCoords.xy).r;
    // 現在描画しようとしているフラグメントの深度値
    float currentDepth = projCoords.z;
    // シャドウ判定 (1.0:シャドウ 0.0:シャドウの外)
    // バイアスを法線とライトの向きから調整する
    float bias = max(0.0001 * (1.0 - dot(normalize(fs_in.fragNormal), u_dirLight.direction)), 0.0001);
    // 現在の深度が最も近いフラグメントの深度より大きければ1.0、小さければ0.0(影になる)
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    vec2 texelSize = 1.0 / textureSize(u_mat.depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_mat.depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{
	// ポリゴン表面の法線（フラグメントシェーダー上で補間されている）
	vec3 N = normalize(fs_in.fragNormal);
	// ポリゴン表面からライト方向へのベクトル
	vec3 L = normalize(-u_dirLight.direction);
	// ポリゴン表面からカメラ方向
	vec3 V = normalize(fs_in.fragViewPos - fs_in.fragWorldPos);
	// -L ベクトルを 法線 N に対して反射したベクトルRを求める
	vec3 R = normalize(reflect(-L, N));

	// フォン反射計算
	vec3 Phong = u_dirLight.ambient;
	float NdotL = dot(L, N);

	// 環境マップサンプリング
	float ratio = 1.00 / 1.309;                                        // 反射率
	vec3 I = normalize(fs_in.fragWorldPos - fs_in.fragViewPos);        // カメラの向きベクトル
	vec3 eR = refract(I, N, ratio);                                    // カメラの向きベクトルと法線から反射ベクトルを生成
	vec3 envMap = texture(u_skybox, eR).rgb * 0.5f;

	// ディフューズ計算
	vec3 Diffuse = u_dirLight.diffuse * texture(u_mat.diffuseMap, fs_in.fragTexCoords).rgb;
	// スペキュラ計算
	vec3 Specular = u_dirLight.specular * pow(max(0.0, dot(R, V)), u_specPower) * texture(u_mat.specularMap, fs_in.fragTexCoords).rgb;
	// アンビエント
	vec3 ambient = u_dirLight.ambient * texture(u_mat.diffuseMap, fs_in.fragTexCoords).rgb;

	// 影成分の算出
	float shadow = ShadowCalculation(fs_in.fragPosLightSpace);

	// GBuffer出力
	//out_gPosition = vec3(fs_in.fragWorldPos.z, fs_in.fragWorldPos.x, -fs_in.fragWorldPos.y);
	out_gPosition = fs_in.fragWorldPos;
	out_gNormal = vec3(fs_in.fragNormal.z, fs_in.fragNormal.x, -fs_in.fragNormal.y);
	// シャドウの逆数を取り、0 = 影の時にディフューズとスペキュラの値がキャンセルされる(影となる)
	out_gAlbedoSpec.rgb = ambient + (1.8 - shadow) * Diffuse * envMap;
	out_gAlbedoSpec.a = (1.8 - shadow) * Specular.r;
	out_gBrightColor = texture(u_mat.emissiveMap, fs_in.fragTexCoords) * 0.095f;     // 0.03f
}