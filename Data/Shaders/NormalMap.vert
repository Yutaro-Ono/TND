//---------------------------------------------------------------------+
// 頂点シェーダ
// 3Dモデル(メッシュ)描画用・法線マップ対応
// 接空間(TangentSpaceとして各座標を定義し、フラグメントへ出力)
//---------------------------------------------------------------------+
#version 330 core
// アトリビュート (VertexAttribPointerにて設定)
layout (location = 0) in vec3 in_Position;              // 頂点座標
layout (location = 1) in vec3 in_Normal;           // 法線ベクトル
layout (location = 2) in vec2 in_TexCoords;        // テクスチャUV座標
layout (location = 3) in vec3 in_Tangent;          // 接ベクトル

// uniform 変数
uniform mat4 uViewProj;              // ビュー行列・プロジェクション行列
uniform mat4 uWorldTransform;                // モデル行列

uniform vec3 uLightPos;             // 光源位置
uniform vec3 uViewPos;              // カメラの視点ベクトル

// out構造体 (フラグメントへ出力する変数をまとめた構造体)
out VS_OUT
{
	vec2 TexCoords;                 // テクスチャ座標
	vec3 FragNormal;
	vec3 FragPos;
	// タンジェント空間内の各座標
	vec3 TangentLightPos;           // ライト(明かり)の座標
	vec3 TangentViewPos;            // ビュー(カメラ)の座標
	vec3 TangentFragPos;            // フラグメント座標

}vs_out;


void main()
{
	
	vec4 pos = vec4(in_Position, 1.0);
	pos = pos * uWorldTransform;
	// 座標変換を行った上で、GLの組み込み変数に代入
	gl_Position = pos * uViewProj;
	vs_out.FragPos = pos.xyz;
	// UV座標をフラグメントシェーダに送る
	vs_out.TexCoords = in_TexCoords;
	vs_out.FragNormal = (vec4(in_Normal, 0.0f) * uWorldTransform).xyz;


	vec3 T, B, N;     // TBN行列

	N = in_Normal;     // 法線を代入
	T = in_Tangent;    // 接空間(TangentSpace)を代入

	mat3 normalMatrix = transpose(mat3(uWorldTransform));
	T = normalize(in_Tangent * normalMatrix);
	N = normalize(in_Normal * normalMatrix);

	// 法線に対する接空間の再直行化
	T = normalize(T - dot(T, N) * N);
	// 接空間ベクトルと法線ベクトルの外積から垂直ベクトルB(BiTangent)を取得
	B = cross(N, T);

	T = normalize(vec3(vec4(T, 0.0f) * uWorldTransform));
	B = normalize(vec3(vec4(B, 0.0f) * uWorldTransform));
	N = normalize(vec3(vec4(N, 0.0f) * uWorldTransform));

	// TBN行列を逆行列として生成
	mat3 TBN = transpose(mat3(T, B, N));

	// 接空間内の座標定義
	vs_out.TangentLightPos = uLightPos * TBN;                                  // 接空間における光源位置
	vs_out.TangentViewPos = uViewPos * TBN;                                    // 接空間におけるビュー座標
	vs_out.TangentFragPos = TBN * vec3(vec4(in_Position, 0.0f) * uWorldTransform);      // 接空間における頂点座標
}