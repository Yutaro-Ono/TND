//----------------------------------------------------------+
// スカイボックス用フラグメントシェーダー
//----------------------------------------------------------+
#version 330 core
// 各バッファへの出力 (GBuffer)
layout (location = 0) out vec3 out_gPos;
layout (location = 1) out vec3 out_gNormal;
layout (location = 2) out vec4 out_gAlbedoSpec;
layout (location = 3) out vec4 out_gBrightColor;

// 頂点からの入力
in VS_OUT
{
	vec3 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                 // 法線（ワールド空間）
	vec3 fragWorldPos;               // 頂点位置（ワールド空間）
}fs_in;

uniform samplerCube skybox;

uniform float u_skyLuminance = 1.5f;     // スカイボックスの輝度

void main()
{
    // アルベド出力
	out_gAlbedoSpec = texture(skybox, fs_in.fragTexCoords);
	// 高輝度出力
	out_gBrightColor = texture(skybox, fs_in.fragTexCoords) * u_skyLuminance;
}