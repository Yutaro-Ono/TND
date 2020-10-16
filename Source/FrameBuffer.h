#pragma once

class FrameBuffer
{

public:

	FrameBuffer();
	~FrameBuffer();

	bool CreateFrameBuffer();     // フレームバッファオブジェクトの作成

	void WriteFrameBuffer();
	void DrawFrameBuffer();


private:

	unsigned int m_FBO;             // フレームバッファオブジェクト

	unsigned int m_VAO, m_VBO;      // 頂点配列オブジェクト、頂点バッファオブジェクト

	unsigned int m_RBO;             // レンダーバッファオブジェクト

	unsigned int m_colorBuffer;     // カラーバッファ

};