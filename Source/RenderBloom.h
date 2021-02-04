//----------------------------------------------------------------------------------------+
//
// HDR(HighDynamicRange)対応のフレームバッファとそれを利用したBloom処理を行う
//
//----------------------------------------------------------------------------------------+
#pragma once
#include <vector>
#include "Math.h"

class RenderBloom
{
public:

	RenderBloom();        // コンストラクタ
	~RenderBloom();       // デストラクタ

	// カラーバッファ・高輝度バッファへの書き込み(専用のシェーダでメッシュの全描画を行う)
	void WriteBuffer(std::vector<class MeshComponent*> in_meshComp, std::vector<class SkeletalMeshComponent*> in_skelComp,
		class CubeMapComponent* in_cubeMapComp, std::vector<class EnvironmentMapComponent*> in_envComp);
	void WriteBuffer(class ParticleManager* in_particle);

	void DrawDownSampling(unsigned int in_brightBuffer);      // 高輝度バッファをダウンサンプリング計算して描画する
	void DrawGaussBlur();                                     // ガウスぼかし効果を適用する

	void DrawBlendBloom(unsigned int in_colorBuffer);         // Bloomを適用した最終結果を描画する


	// ガウス計算関数
	float GaussianDistribution(const Vector2& in_pos, float in_rho);    // ガウスぼかしの重みを算出する (in_rho = 偏差)
	// ガウスぼかし計算
	void CalcGaussBlurParam(int in_w, int in_h, Vector2 in_dir, float in_deviation, Vector3* in_offset);

	// バッファの取得
	unsigned int GetColorBuffer() { return m_defaultBuffer; }
	unsigned int GetBrightBuffer() { return m_brightBuffer; }

	// フェードイン・フェードアウト
	bool FadeOut(float in_val, float in_deltaTime);
	bool FadeIn(float in_val, float in_deltaTime);
	// ホワイトイン・ホワイトアウト
	bool WhiteOut(float in_val, float in_deltaTime);
	bool WhiteIn(float in_val, float in_deltaTime);


	void SetExposureVal(float in_exp) { m_exposure = in_exp; m_saveExposure = m_exposure; }     // 露出
	void SetGamma(float in_gamma) { m_gamma = in_gamma; }          // ガンマコレクション

private:

	// バッファの生成処理
	bool CreateHDRFBO();
	// カラーバッファの生成
	void CreateColorBuffer(unsigned int& in_colorBuffer, const unsigned int in_attachNum);
	// ダウンサンプリング・ぼかし用FBOの生成
	void CreateBlurFBO();


	//----------------------------+
	// メンバ変数
	//----------------------------+
	unsigned int m_hdrFBO;                             // HDR(HighDynamicRange)対応のフレームバッファ (色値を浮動小数点型にすることで、色域を精細化する)
	unsigned int m_rbo;                                // 描画バッファ

	// マルチレンダーターゲット
	unsigned int m_defaultBuffer;                      // 通常のカラーバッファ(HDR対応)
	unsigned int m_brightBuffer;                       // 高輝度域を保存するバッファ
	unsigned int m_attachments[2];                     // マルチレンダーターゲット明示用

	// ダウンサンプリング・ぼかし用のFBOとテクスチャ配列
	std::vector<unsigned int> m_blurFBO;
	std::vector<unsigned int> m_blurBufferTex;

	class Shader* m_multiRenderTargetShader;           // HDR対応マルチターゲットシェーダ (Mesh)
	class Shader* m_multiRenderSkinShader;             // HDR対応マルチターゲットシェーダ (SkinMesh)
	class Shader* m_multiRenderCubeMapShader;          // HDR対応マルチターゲットシェーダ (CubeMap)
	class Shader* m_multiRenderEnvironmentShader;      // HDR対応マルチターゲットシェーダ (EnvironmentMap)
	class Shader* m_hdrBloomShader;                    // Bloomシェーダ
	class Shader* m_downSamplingShader;                // 縮小バッファ用シェーダ
	class Shader* m_gaussShader;                       // ガウスぼかし計算用シェーダ

	float m_exposure;                                  // 露出量
	float m_saveExposure;                              // フェードアウト処理時の露出保存用

	float m_gamma;                                     // ガンマコレクション
};