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

	void LinkUniformBuffers();

private:

	bool CreateGBuffer();             // GBufferの作成
	bool CreateLightBuffer();         // ライトバッファの作成



	//-----------------------------+
	// メンバ変数
	//-----------------------------+
	class Renderer* m_renderer;           // レンダラークラスへのポインタ

	// メッシュシェーダ
	class Shader* m_simpleMeshShader;       // マップ描画用シンプルシェーダ
	class Shader* m_meshShader;             // GBuffer対応シェーダ
	class Shader* m_skinShader;             // GBuffer対応スキンシェーダ
	class Shader* m_skyBoxShader;           // GBuffer対応スカイボックスシェーダ
	class Shader* m_envShader;              // GBuffer対応環境マップシェーダ
	class Shader* m_carShader;              // GBuffer対応車用シェーダ
	class Shader* m_lightGlassShader;       // 光るガラス(車のフロントライトなど)シェーダ

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
	unsigned int m_gPos;                  // GBuffer用3D空間座標
	unsigned int m_gNormal;               // GBuffer用法線ベクトル
	unsigned int m_gAlbedoSpec;           // GBuffer用アルベド(RGB)＆スペキュラ(A)
	unsigned int m_gEmissive;             // GBuffer用輝度(※エミッシブ)バッファ
	unsigned int m_gAttachments[4];       // GBufferアタッチメント (バッファ数分確保)
	unsigned int m_gRBO;                  // G-Buffer用の描画バッファオブジェクト

	// ライトバッファ (光源処理用)
	unsigned int m_lightFBO;              // ライトバッファ
	// ライトバッファに関連付ける出力
	unsigned int m_lightHDR;              // 光源処理用のHDR対応バッファ (浮動小数点)
	unsigned int m_lightHighBright;         // ライト用高輝度バッファ
	unsigned int m_lightRBO;              // ライト用レンダーバッファ
	unsigned int m_lightAttachments[2];

	// uniformバッファ (GBuffer用)
	unsigned int m_uboGBuffer;

};