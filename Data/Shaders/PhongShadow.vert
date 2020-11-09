//---------------------------------------------------------------------+
// フォンライティング (シャドウ付き)
//---------------------------------------------------------------------+
#version 330 core
// attribute
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;

// ワールド変換行列・ビュープロジェクション行列・ライトスペース
uniform mat4 u_worldTransform;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_lightSpaceMatrix;     

// フラグメントへの出力
out VS_OUT
{
	vec2 fragTexCoords;          // テクスチャ座標
	vec3 fragNormal;            // ワールドスペース上の法線
	vec3 fragWorldPos;          // ワールドスペース上の座標
	vec4 fragPosLightSpace;     // ライトスペース上の座標
}vs_out;


void main()
{


	vec4 pos = vec4(a_pos, 1.0) * u_worldTransform;
	vs_out.fragWorldPos     = pos.xyz;
	gl_Position = pos * u_view * u_projection;
	vs_out.fragNormal      =  a_normal * mat3(transpose(inverse(u_worldTransform)));
	vs_out.fragTexCoords   = a_texCoords;

	// ワールド座標の頂点をライトスペースに変換して保存
	vs_out.fragPosLightSpace = vec4(vs_out.fragWorldPos, 1.0) * u_lightSpaceMatrix;

}
