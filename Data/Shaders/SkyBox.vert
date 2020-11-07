//-------------------------------------------------------+
// スカイボックス用頂点シェーダ
//-------------------------------------------------------+
#version 330 core
// attribute
layout (location = 0) in vec3 a_Pos;

out vec3 TexCoords;

uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	// テクスチャ座標にローカル位置ベクトルを設定
	TexCoords = vec3(a_Pos.y, -a_Pos.z, a_Pos.x);

	vec4 pos = u_Proj * u_View * vec4(a_Pos, 1.0);

	gl_Position = vec4(-pos.x, -pos.y, pos.w, pos.w);     // z成分をwで置き換える(最奥表示)


}