//---------------------------------------------------+
// ガウスぼかしシェーダ
//---------------------------------------------------+
#version 330 core
// 出力カラー
out vec4 out_fragColor;
// 入力テクスチャ
in vec2 TexCoords;

uniform sampler2D u_blurSource;

// ガウスぼかしのパラメータ構造体
struct GaussBlurParam
{
	int sampleCount;
	vec3 offset[16];
};
uniform GaussBlurParam u_param;

void main()
{
	vec4 result = vec4(0.0f);
	for(int i = 0; i < u_param.sampleCount; i++)
	{
		// ガウスの重みからテクスチャカラー合成を行う
		result += u_param.offset[i].z * texture(u_blurSource, TexCoords + u_param.offset[i].xy);
	}
	result.w = 1.0f;
	out_fragColor = result;
}