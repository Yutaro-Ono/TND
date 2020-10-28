#version 330 core

// attribute
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// ワールド変換行列・ビュープロジェクション行列・ライトスペース
uniform mat4 uWorldTransform;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uViewProj;
uniform mat4 uLightSpaceMatrix;     

// フラグメントへの出力
out vec2 fragTexCoord;          // テクスチャ座標
out vec3 fragNormal;            // ワールドスペース上の法線
out vec3 fragWorldPos;          // ワールドスペース上の座標
out vec4 fragPosLightSpace;     // ライトスペース上の座標

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	pos = pos * uWorldTransform;
	fragWorldPos     = pos.xyz;
	gl_Position = pos * uView * uProjection;
	fragNormal      =  (vec4(inNormal, 0.0) * uWorldTransform).xyz;
	fragTexCoord   = inTexCoord;
	fragPosLightSpace = vec4(fragWorldPos, 1.0) * uLightSpaceMatrix;


}
