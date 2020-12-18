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

	bool Initialize();                // 初期化処理 (各種フレームバッファを作成する)

	void DrawGBuffer();               // GBufferへの書き込み処理
	void DrawLightPass();             // GBufferからライティング計算を行う
	void Draw();                      // GBufferに書き込まれた情報の描画

private:

	bool CreateGBuffer();             // GBufferの作成
	bool CreateLightBuffer();         // ライトバッファの作成



	//-----------------------------+
	// メンバ変数
	//-----------------------------+
	class Renderer* m_renderer;           // レンダラークラスへのポインタ

	// メッシュシェーダ
	class Shader* m_gBufferShader;        // GBuffer対応シェーダ
	class Shader* m_gBufferSkinShader;    // GBuffer対応スキンシェーダ
	class Shader* m_gBufferSkyBoxShader;  // GBuffer対応スカイボックスシェーダ
	class Shader* m_gBufferEnvShader;     // GBuffer対応環境マップシェーダ
	class Shader* m_gBufferCarShader;     // GBuffer対応車用シェーダ

	// ライトシェーダ
	class Shader* m_pointLightShader;
	class Shader* m_lightSphereShader;
	class Shader* m_directionalLightShader;
	class Shader* m_spotLightShader;

	class Shader* m_bloomSpriteShader;
	class Shader* m_bloomWorldSpriteShader;

	class Shader* m_screenShader;         // スクリーン出力用シェーダ

	// GBuffer
	unsigned int m_gBuffer;               // G-Buffer (3要素を持つフレームバッファ)
    // G-Bufferに関連付ける各種出力
	unsigned int m_gPos;                  // 3D空間座標
	unsigned int m_gNormal;               // 法線ベクトル
	unsigned int m_gAlbedoSpec;           // アルベド(RGB)＆スペキュラ(A)
	unsigned int m_gRBO;                  // G-Bufferに紐付ける描画バッファ
	unsigned int m_attachments[3];

	// ライトバッファ (光源処理用)
	unsigned int m_lightFBO;              // ライトバッファ
	// ライトバッファに関連付ける出力
	unsigned int m_lightHDR;              // 光源処理用のHDR対応バッファ (浮動小数点)
	unsigned int m_lightBrightBuffer;     // ライト用高輝度バッファ
	unsigned int m_lightRBO;              // ライト用レンダーバッファ
	unsigned int m_lightAttachments[2];

};