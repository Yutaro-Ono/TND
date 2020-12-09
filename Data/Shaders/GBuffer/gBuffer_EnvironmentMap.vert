//---------------------------------------------+
// 環境マッピング (マルチレンダー対応)
//---------------------------------------------+
#version 330 core
// attribute
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

// 出力
// フラグメントへの出力
out VS_OUT
{
	vec3 normal;            // ワールドスペース上の法線
	vec3 fragWorldPos;          // ワールドスペース上の座標
}vs_out;

// 入力
uniform mat4 u_worldTransform;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform;

	vs_out.normal = a_normal * mat3(transpose(u_worldTransform));

	vs_out.normal = vec3(vs_out.normal.y, -vs_out.normal.z, vs_out.normal.x);

	vs_out.fragWorldPos = vec3(pos.y, -pos.z, pos.x);                                                 // ワールド上の位置ベクトルを出力

	gl_Position = pos * u_view * u_projection;
}