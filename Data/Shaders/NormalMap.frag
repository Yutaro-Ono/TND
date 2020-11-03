//---------------------------------------------------------------------+
// フラグメントシェーダ
// 3Dモデル(メッシュ)描画用・法線マップ対応
//---------------------------------------------------------------------+
#version 330 core

out vec4 out_fragColor;       // 出力先：カラー

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


uniform Material u_mat;


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
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
	// フラグメントカラーを出力
    out_fragColor = vec4(ambient + diffuse + specular, 1.0);
}