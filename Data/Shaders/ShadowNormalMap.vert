#version 330 core

// attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_tangent;

// ワールド変換行列・ビュープロジェクション行列・ライトスペース
uniform mat4 u_worldTransform;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_lightSpaceMatrix;   
// 光源座標・カメラ座標
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

// フラグメントへの出力
out VS_OUT
{
	vec2 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                // ワールドスペース上の法線
	vec3 fragWorldPos;              // ワールドスペース上の座標
	vec3 fragViewPos;               // カメラ座標
	vec4 fragPosLightSpace;         // ライトスペース上の座標
	// タンジェント空間内の各座標
	vec3 TangentLightPos;           // ライト(明かり)の座標
	vec3 TangentViewPos;            // ビュー(カメラ)の座標
	vec3 TangentFragPos;            // フラグメント座標

}vs_out;


void main()
{
	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform;
	vs_out.fragWorldPos     = pos.xyz;
	gl_Position = pos * u_view * u_projection;
	vs_out.fragNormal      =  a_normal * mat3(transpose(inverse(u_worldTransform)));
	vs_out.fragTexCoords   = a_texCoords;
	vs_out.fragViewPos = u_viewPos;
	// ワールド座標の頂点をライトスペースに変換して保存
	vs_out.fragPosLightSpace = vec4(vs_out.fragWorldPos, 1.0) * u_lightSpaceMatrix;


	vec3 T, B, N;     // TBN行列

	N = a_normal;     // 法線を代入
	T = a_tangent;    // 接空間(TangentSpace)を代入

	// 法線に対する接空間の再直行化
	T = normalize(T - dot(T, N) * N);
	// 接空間ベクトルと法線ベクトルの外積から垂直ベクトルB(BiTangent)を取得
	B = cross(N, T);

	T = normalize(vec3(vec4(T, 0.0f) * u_worldTransform));
	B = normalize(vec3(vec4(B, 0.0f) * u_worldTransform));
	N = normalize(vec3(vec4(N, 0.0f) * u_worldTransform));

	// TBN行列を逆行列として生成
	mat3 TBN = transpose(mat3(T, B, N));

	// 接空間内の座標定義
	vs_out.TangentLightPos = u_lightPos * TBN;                                     // 接空間における光源位置
	vs_out.TangentViewPos = u_viewPos * TBN;                                       // 接空間におけるビュー座標
	vs_out.TangentFragPos = vec3(vec4(a_pos, 0.0f) * u_worldTransform) * TBN;      // 接空間における頂点座標



}