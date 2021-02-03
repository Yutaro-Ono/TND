#version 420
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

// uniformバッファブロック
// 0.行列
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};


uniform mat4 u_worldTransform;

// フラグメントへの出力
out VS_OUT
{
	vec2 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                 // 法線（ワールド空間）
	vec3 fragWorldPos;               // 頂点位置（ワールド空間）
}vs_out;

void main()
{
	// 頂点座標をワールド空間に変換
	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform;
	
	gl_Position = pos * u_view * u_projection;
	// 各種出力にセット
	vs_out.fragWorldPos = vec3(vec4(a_pos, 1.0) * u_worldTransform);
	vs_out.fragNormal = a_normal * mat3(transpose(inverse(u_worldTransform)));
	vs_out.fragTexCoords = a_texCoords;
}
