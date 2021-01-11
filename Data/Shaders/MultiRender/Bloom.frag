//----------------------------------------------------------------------------------------+
// HDR最終ブレンド
// 標準カラーバッファとBloom効果を適用したバッファを合成して出力する
//----------------------------------------------------------------------------------------+
#version 330 core
// カラー出力
out vec4 out_fragColor;

// 頂点シェーダからの入力
in vec2 TexCoords;

uniform sampler2D u_scene;      // シーンのテクスチャ (マルチレンダーターゲットの1番目：標準カラーバッファ)

// 縮小＆ガウスぼかしを適用したBloomテクスチャ
uniform sampler2D u_bloom1;
uniform sampler2D u_bloom2;
uniform sampler2D u_bloom3;
uniform sampler2D u_bloom4;
uniform sampler2D u_bloom5;

uniform float u_exposure;        // 露出値 (0で画面が真っ暗になる)

void main()
{
	vec3 hdrColor    = texture(u_scene, TexCoords).rgb;
    vec3 bloomColor1 = texture(u_bloom1, TexCoords).rgb;
    vec3 bloomColor2 = texture(u_bloom2, TexCoords).rgb;
    vec3 bloomColor3 = texture(u_bloom3, TexCoords).rgb;
    vec3 bloomColor4 = texture(u_bloom4, TexCoords).rgb;
    vec3 bloomColor5 = texture(u_bloom4, TexCoords).rgb;
    hdrColor += bloomColor1 + bloomColor2 + bloomColor3 + bloomColor4 + bloomColor5; 
  
    // 露出トーンマッピング
    vec3 mapped = vec3(1.0) - exp(-hdrColor * u_exposure);
    
    // ガンマコレクション
    //const float gamma = 2.2;
    const float gamma = 0.125;

    mapped = pow(mapped, vec3(1.0 / gamma));  
    out_fragColor = vec4(mapped, 1.0);
}