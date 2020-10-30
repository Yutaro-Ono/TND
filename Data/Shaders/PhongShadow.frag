#version 330 core

// 頂点シェーダーからの入力

in vec2 fragTexCoord;              // テクスチャ座標
in vec3 fragNormal;                // 法線（ワールド空間）
in vec3 fragWorldPos;              // 頂点位置（ワールド空間）
in vec4 fragPosLightSpace;         // 頂点位置 (ライト空間)

// 出力カラー（出力ピクセルカラー）
out vec4 outColor;

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
	vec3 direction;      // ライト方向
	vec3 ambient;        // アンビエント
	vec3 diffuse;        // ディフューズ色
	vec3 specular;       // スペキュラー色
};
// ディレクショナルライト
uniform DirectionalLight u_dirLight;

// ライティング用変数
// カメラ座標
uniform vec3 uCameraPos;
// ポリゴン表面のスペキュラー強度
uniform float uSpecPower;


// シャドウの計算
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // パースペクティブ除算(遠近感を出すための計算だが、今回はw成分が1のため意味なし)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 正規化デバイス座標をテクスチャ座標と合わせるため、[-1〜1]を[0〜1]の範囲に変換
    projCoords = projCoords * 0.5 + 0.5;
    // シャドウマップよりライトに最も近いフラグメントの深度値を得る
    float closestDepth = texture(u_mat.depthMap, projCoords.xy).r;
    // 現在描画しようとしているフラグメントの深度値
    float currentDepth = projCoords.z;
    // シャドウ判定 (1.0:シャドウ 0.0:シャドウの外)
    // バイアスを法線とライトの向きから調整する
    float bias = max(0.0001 * (1.0 - dot(normalize(fragNormal), u_dirLight.direction)), 0.0001);
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
	vec3 N = normalize(fragNormal);
	// ポリゴン表面からライト方向へのベクトル
	vec3 L = normalize(-u_dirLight.direction);
	// ポリゴン表面からカメラ方向
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// -L ベクトルを 法線 N に対して反射したベクトルRを求める
	vec3 R = normalize(reflect(-L, N));
	// フォン反射計算
	vec3 Phong = u_dirLight.ambient;
	float NdotL = dot(N, L);

	// アンビエント計算
	vec3 ambient = u_dirLight.ambient * vec3(texture(u_mat.diffuseMap, fragTexCoord));
	// ディフューズ計算
	vec3 norm = normalize(fragNormal);
	float diff = max(dot(norm, -u_dirLight.direction), 0.0);
	//float diff = max(NdotL, 0.0);
	vec3 diffuse = u_dirLight.diffuse * diff * vec3(texture(u_mat.diffuseMap, fragTexCoord));

	// スペキュラ計算
	vec3 viewDir = normalize(uCameraPos - fragWorldPos);
	vec3 reflectDir = reflect(u_dirLight.direction, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	//float spec = pow(max(0.0, dot(R, V)), uSpecPower);
	vec3 specular = u_dirLight.specular * spec * vec3(texture(u_mat.specularMap, fragTexCoord));

	// 影成分の算出
	float shadow = ShadowCalculation(fragPosLightSpace);
	// シャドウの逆数を取り、0 = 影の時にディフューズとスペキュラの値がキャンセルされる(影となる)
	vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);

	// 最終カラーを出力
	outColor = vec4(result, 1.0);
}
