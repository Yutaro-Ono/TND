//-------------------------------------------------------------------------------------------+
//
// 遅延レンダリングクラス (GBufferに各描画結果を保存・GBufferに対して光源処理を行う)
//
//-------------------------------------------------------------------------------------------+
#pragma once

class DefferedRenderer
{

public:

	DefferedRenderer(class Renderer* in_renderer);
	~DefferedRenderer();

	bool Initialize();

	void Draw();                      // Rendererクラスに登録されているオブジェクトの描画

private:

	bool CreateGBuffer();             // GBufferの作成
	bool CreateLightBuffer();         // ライトバッファの作成


	//-----------------------------+
	// メンバ変数
	//-----------------------------+
	class Renderer* m_renderer;           // レンダラークラスへのポインタ

	// GBuffer
	unsigned int m_gBuffer;               // G-Buffer (3要素を持つフレームバッファ)
    // G-Bufferに関連付ける各種出力
	unsigned int m_gPos;                  // 3D空間座標
	unsigned int m_gNormal;               // 法線ベクトル
	unsigned int m_gAlbedoSpec;           // アルベド(RGB)＆スペキュラ(A)
	unsigned int m_gRBO;                  // G-Bufferに紐付ける描画バッファ

	// ライトバッファ (光源処理用)
	unsigned int m_lightFBO;              // ライトバッファ
	// ライトバッファに関連付ける出力
	unsigned int m_lightHDR;              // 光源処理用のHDR対応バッファ (浮動小数点)
	unsigned int m_lightRBO;              // ライト用レンダーバッファ
};