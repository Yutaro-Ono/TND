//----------------------------------------------------------------------------------------+
//
// HDR(HighDynamicRange)対応のフレームバッファとそれを利用したBloom処理を行う
//
//----------------------------------------------------------------------------------------+
#pragma once
#include <vector>

class RenderBloom
{
public:

	RenderBloom();        // コンストラクタ
	~RenderBloom();       // デストラクタ


private:

	// バッファの生成処理
	bool CreateHDRFBO();
	// カラーバッファの生成
	void CreateColorBuffer(unsigned int in_colorBuffer, const unsigned int in_attachNum);



	unsigned int m_hdrFBO;              // HDR(HighDynamicRange)対応のフレームバッファ (色値を浮動小数点型にすることで、色域を精細化する)
	unsigned int m_rbo;                 // 描画バッファ

	// マルチレンダーターゲット
	unsigned int m_defaultBuffer;       // 通常のカラーバッファ(HDR対応)
	unsigned int m_brightBuffer;        // 高輝度域を保存するバッファ
	unsigned int m_attachments[2];      // マルチレンダーターゲット明示用

	// ダウンサンプリング・ぼかし用のFBOとテクスチャ配列
	std::vector<unsigned int> m_blurFBO;
	std::vector<unsigned int> m_blurBufferTex;


};