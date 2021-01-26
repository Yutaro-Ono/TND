//------------------------------------------+
// マップ描画用の単色出力
//------------------------------------------+
#version 330 core
out vec4 outColor;

in vec2 fragTexCoord;

// 指定色
uniform vec3 u_mapColor;

void main()
{
    outColor = vec4(u_mapColor, 1.0f);
}
