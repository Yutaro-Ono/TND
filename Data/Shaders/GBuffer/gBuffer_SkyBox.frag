//----------------------------------------------------------+
// スカイボックス用フラグメントシェーダー
//----------------------------------------------------------+
#version 330 core
// 各バッファへの出力 (GBuffer)
layout (location = 0) out vec3 out_gPos;
layout (location = 1) out vec3 out_gNormal;
layout (location = 2) out vec4 out_gAlbedoSpec;

// 頂点からの入力
in VS_OUT
{
	vec3 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                 // 法線（ワールド空間）
	vec3 fragWorldPos;               // 頂点位置（ワールド空間）
}fs_in;

uniform samplerCube skybox;

void main()
{
    // アルベド出力
	out_gAlbedoSpec = texture(skybox, fs_in.fragTexCoords);
}