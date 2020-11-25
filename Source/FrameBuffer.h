#pragma once
#include <vector>

class FrameBuffer
{

public:


	enum POST_EFFECT_TYPE
	{
		NONE = 0,
		INVERT_COLOR,
		GREY_SCALE,
		CERNEL,
		BLUR,
		SHARP_EDGE,
		ALL_NUM
	};


	FrameBuffer();
	~FrameBuffer();

	bool CreateFrameBuffer();     // フレームバッファオブジェクトの作成

	void WriteFrameBuffer();
	void DrawFrameBuffer();

	void DebugFrameBuffer();

private:

	unsigned int m_FBO;             // フレームバッファオブジェクト

	unsigned int m_RBO;             // レンダーバッファオブジェクト

	unsigned int m_colorBuffer;     // カラーバッファ


	int m_shaderNum;
	std::vector<class Shader*> m_postEffectShaders;      // ポストエフェクト用シェーダ
	class Shader* m_activeShader;                        // 現在アクティブなシェーダ
};