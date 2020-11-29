//-------------------------------------------------------------------------------------------+
// gBuffer：頂点の情報から、各要素を指定のバッファに保存する (ベーシックメッシュ)
//-------------------------------------------------------------------------------------------+
#version 330 core
layout (location = 0) out vec3 gPosition;   // G-Bufferの位置テクスチャに出力
layout (location = 1) out vec3 gNormal;     // G-Bufferの法線テクスチャに出力
layout (location = 2) out vec4 gAlbedoSpec; // G-Bufferのアルベド＆スペキュラ強度テクスチャに出力

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
};
uniform Material u_mat;

void main()
{
    // フラグメントの座標をgBufferに保存
    gPosition = fs_in.fragWorldPos;

    // フラグメントごとの法線ベクトルをgBufferに保存
    gNormal = normalize(fs_in.fragNormal);

    // ディフューズカラーをアルベドのrgbに保存
    gAlbedoSpec.rgb = texture(u_mat.diffuseMap, fs_in.fragTexCoords).rgb;

    // スペキュラ強度をアルベドのa要素に保存
    gAlbedoSpec.a = texture(u_mat.specularMap, fs_in.fragTexCoords).r;

}