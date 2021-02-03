//-------------------------------------------------------+
// スカイボックス用頂点シェーダ (GBufferに対応)
//-------------------------------------------------------+
#version 420
// attribute
layout (location = 0) in vec3 a_pos;

// uniformバッファブロック
// 0.行列
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};


// フラグメントへの出力
out VS_OUT
{
	vec3 fragTexCoords;              // テクスチャ座標
	vec3 fragNormal;                 // 法線（ワールド空間）
	vec3 fragWorldPos;               // 頂点位置（ワールド空間）
}vs_out;

uniform mat4 u_invView;

void main()
{
	// テクスチャ座標にローカル位置ベクトルを設定
	vs_out.fragTexCoords = vec3(a_pos.y, -a_pos.z, a_pos.x);

	vec4 pos = u_projection * u_invView * vec4(a_pos, 1.0);

	gl_Position = vec4(-pos.x, -pos.y, pos.w, pos.w);     // z成分をwで置き換える(最奥表示)

}