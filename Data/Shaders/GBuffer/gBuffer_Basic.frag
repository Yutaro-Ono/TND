//-------------------------------------------------------------------------------------------+
// gBuffer：頂点の情報から、各要素を指定のバッファに保存する (ベーシックメッシュ)
//-------------------------------------------------------------------------------------------+
#version 330 core
layout (location = 0) out vec3 out_gPosition;   // G-Bufferの位置テクスチャに出力
layout (location = 1) out vec3 out_gNormal;     // G-Bufferの法線テクスチャに出力
layout (location = 2) out vec4 out_gAlbedoSpec; // G-Bufferのアルベド＆スペキュラ強度テクスチャに出力
layout (location = 3) out vec4 out_gEmissive;   // B-Bufferのエミッシブ出力

// 頂点シェーダーからの入力受け取り
in VS_OUT
{
	vec2 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                // 法線（ワールド空間）
	vec3 fragWorldPos;              // 頂点位置（ワールド空間）
}fs_in;

// テクスチャサンプリング用構造体
struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
    sampler2D normalMap;
    sampler2D emissiveMap;
};
uniform Material u_mat;

uniform float u_intensity;      // 高輝度部分の明るさ


void main()
{
    // フラグメントの座標をgBufferに保存
    out_gPosition = fs_in.fragWorldPos;
    // フラグメントごとの法線ベクトルをgBufferに保存
    out_gNormal = normalize(fs_in.fragNormal);
    // ディフューズカラーをアルベドのrgbに保存
    out_gAlbedoSpec.rgb = texture(u_mat.diffuseMap, fs_in.fragTexCoords).rgb;
    // スペキュラ強度をアルベドのa要素に保存
    out_gAlbedoSpec.a = texture(u_mat.specularMap, fs_in.fragTexCoords).r;
    // エミッシブ情報を保存
    out_gEmissive = texture(u_mat.emissiveMap, fs_in.fragTexCoords) * u_intensity;
}