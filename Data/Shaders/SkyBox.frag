//----------------------------------------------------------+
// スカイボックス用フラグメントシェーダー
//----------------------------------------------------------+
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    // スカイボックス用テクスチャをサンプリング
	FragColor = texture(skybox, TexCoords);
}