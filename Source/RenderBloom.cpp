//----------------------------------------------------------------------------------------+
//
// HDR(HighDynamicRange)対応のフレームバッファとそれを利用したBloom処理を行う
//
//----------------------------------------------------------------------------------------+
#include "RenderBloom.h"
#include "GameMain.h"
#include "Renderer.h"
#include "GameConfig.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "ShadowMap.h"
#include "Shader.h"
#include <iostream>

const unsigned int maxLevelNum = 5;

// コンストラクタ
RenderBloom::RenderBloom()
	:m_hdrBloomShader(nullptr)
	,m_downSamplingShader(nullptr)
{
	// Bloom用FBOとテクスチャを生成
	if (!CreateHDRFBO())
	{
		std::cout << "HDRバッファの生成に失敗" << std::endl;
	}
	CreateBlurFBO();

	// スクリーンを覆うための頂点配列作成
	float quadVertices[] = {
		// ポジション   // テクスチャ座標
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	// 頂点オブジェクトの作成
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	// アトリビュート指定
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	// シェーダのロード
	m_hdrBloomShader = new Shader();
	m_hdrBloomShader->Load("Data/Shaders/MultiRender/BloomNormShadowMesh.vert", "Data/Shaders/MultiRender/BloomNormShadowMesh.frag");
	// ダウンサンプリング用シェーダ
	m_downSamplingShader = new Shader();
	m_downSamplingShader->Load("Data/Shaders/FrameBufferScreen.vert", "Data/Shaders/MultiRender/DownSampling.frag");

}

// デストラクタ
RenderBloom::~RenderBloom()
{
	delete m_hdrBloomShader;
	delete m_downSamplingShader;

	for (auto fbo : m_blurFBO)
	{
		glDeleteFramebuffers(1, &fbo);
	}
	m_blurFBO.clear();

	for (auto tex : m_blurBufferTex)
	{
		glDeleteTextures(1, &tex);
	}
	m_blurBufferTex.clear();

	glDeleteFramebuffers(1, &m_hdrFBO);
	glDeleteRenderbuffers(1, &m_rbo);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

// カラーバッファ・高輝度バッファへの書き込み(専用のシェーダでメッシュの全描画を行う)
void RenderBloom::WriteBuffer(std::vector<class MeshComponent*> in_meshComp)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// ビューポートを画面サイズに戻す
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ライト空間の各行列を定義
	Matrix4 lightSpace, lightView, lightProj;
	lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight().position, RENDERER->GetDirectionalLight().target, Vector3::UnitZ);
	lightSpace = lightView * lightProj;

	// シャドウシェーダのアクティブ化・uniformへのセット
	m_hdrBloomShader->SetActive();
	m_hdrBloomShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_hdrBloomShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_hdrBloomShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_hdrBloomShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_hdrBloomShader->SetMatrixUniform("u_view", RENDERER->GetViewMatrix());
	m_hdrBloomShader->SetMatrixUniform("u_projection", RENDERER->GetProjectionMatrix());
	m_hdrBloomShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_hdrBloomShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_hdrBloomShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	// サンプリング用テクスチャセット
	m_hdrBloomShader->SetInt("u_mat.diffuseMap", 0);
	m_hdrBloomShader->SetInt("u_mat.specularMap", 1);
	m_hdrBloomShader->SetInt("u_mat.normalMap", 2);
	m_hdrBloomShader->SetInt("u_mat.depthMap", 3);

	// シャドウシェーダによるメッシュ描画
	for (auto mesh : in_meshComp)
	{
		mesh->Draw(m_hdrBloomShader);
	}
}

// カラーバッファ・高輝度バッファへの書き込み(専用のシェーダでメッシュの全描画を行う)
void RenderBloom::WriteBuffer(std::vector<class SkeletalMeshComponent*> in_skelComp)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// ビューポートを画面サイズに戻す
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ライト空間の各行列を定義
	Matrix4 lightSpace, lightView, lightProj;
	lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight().position, RENDERER->GetDirectionalLight().target, Vector3::UnitZ);
	lightSpace = lightView * lightProj;

	// シャドウシェーダのアクティブ化・uniformへのセット
	m_hdrBloomShader->SetActive();
	m_hdrBloomShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_hdrBloomShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_hdrBloomShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_hdrBloomShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_hdrBloomShader->SetMatrixUniform("u_view", RENDERER->GetViewMatrix());
	m_hdrBloomShader->SetMatrixUniform("u_projection", RENDERER->GetProjectionMatrix());
	m_hdrBloomShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_hdrBloomShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_hdrBloomShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	// サンプリング用テクスチャセット
	m_hdrBloomShader->SetInt("u_mat.diffuseMap", 0);
	m_hdrBloomShader->SetInt("u_mat.specularMap", 1);
	m_hdrBloomShader->SetInt("u_mat.normalMap", 2);
	m_hdrBloomShader->SetInt("u_mat.depthMap", 3);

	// シャドウシェーダによるメッシュ描画
	for (auto mesh : in_skelComp)
	{
		mesh->Draw(m_hdrBloomShader);
	}
}

// 高輝度バッファをダウンサンプリング計算して描画する
void RenderBloom::DrawDownSampling()
{
	glDisable(GL_DEPTH_TEST);

	unsigned int renderSource = m_brightBuffer;   // ダウンサンプリングターゲット (高輝度バッファ)

	int reduceX = GAME_CONFIG->GetScreenWidth();
	int reduceY = GAME_CONFIG->GetScreenHeight();

	// 縮小バッファの奇数番目にダウンサンプリング結果を出力する
	for (int i = 0; i < maxLevelNum; i++)
	{
		reduceX /= 2;
		reduceY /= 2;
		// 描画FBOに奇数番を指定
		glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO[i * 2 + 1]);
		// Viewportの調整
		glViewport(0, 0, reduceX, reduceY);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// テクスチャに高輝度バッファをセットし、シェーダ内でダウンサンプリングした結果を出力する
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderSource);
		m_downSamplingShader->SetActive();
		m_downSamplingShader->SetInt("u_screenTex", 0);

		// 描画する
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		renderSource = m_blurBufferTex[i * 2 + 1];
	}
	
}

// ガウスぼかしの重みを算出する (in_rho = 偏差)
float RenderBloom::GaussianDistribution(const Vector2& in_pos, float in_rho)
{
	return exp(-(in_pos.x * in_pos.x + in_pos.y * in_pos.y) / (2.0f * in_rho * in_rho));
}

void RenderBloom::CalcGaussBlurParam(int in_w, int in_h, Vector2 in_dir, float in_deviation, Vector3* in_offset)
{
	auto tu = 1.0f / float(in_w);
	auto tv = 1.0f / float(in_h);
	in_offset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), in_deviation);
	auto total_weight = in_offset[0].z;
	in_offset[0].x = 0.0f;
	in_offset[0].y = 0.0f;
	for (auto i = 1; i < 8; ++i)
	{
		int nextpos = (i - 1) * 2 + 1;
		in_offset[i].x = in_dir.x * tu * nextpos;
		in_offset[i].y = in_dir.y * tv * nextpos;
		in_offset[i].z = GaussianDistribution(in_dir * float(nextpos), in_deviation);
		total_weight += in_offset[i].z * 2.0f;
	}
	for (auto i = 0; i < 8; ++i)
	{
		in_offset[i].z /= total_weight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		in_offset[i].x = -in_offset[i - 7].x;
		in_offset[i].y = -in_offset[i - 7].y;
		in_offset[i].z = in_offset[i - 7].z;
	}
}

// バッファの生成
bool RenderBloom::CreateHDRFBO()
{
	glGenFramebuffers(1, &m_hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	// カラーバッファの生成とアタッチ
	CreateColorBuffer(m_defaultBuffer, 0);
	CreateColorBuffer(m_brightBuffer, 1);
	// レンダーバッファ生成
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	// フレームバッファにアタッチ
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	// GLにマルチレンダリングを行うことを明示
	m_attachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_attachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	// バインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

// カラーバッファの生成
void RenderBloom::CreateColorBuffer(unsigned int in_colorBuffer, const unsigned int in_attachNum)
{
	glGenTextures(1, &in_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, in_colorBuffer);
	// スクリーンサイズのテクスチャを生成 (浮動小数点型)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	// テクスチャフィルタリング・ラッピング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// フレームバッファにアタッチ
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + in_attachNum, GL_TEXTURE_2D, in_colorBuffer, 0);

}

// ダウンサンプリング・ぼかし用のFBOを作成する
// ガウスぼかしにて縦横2枚ずつ使用するので、段階ごとに2枚ずつ作成
void RenderBloom::CreateBlurFBO()
{
	int w = GAME_CONFIG->GetScreenWidth();
	int h = GAME_CONFIG->GetScreenHeight();

	// FBOとテクスチャIDの枠を確保する
	m_blurFBO.resize(maxLevelNum * 2);
	m_blurBufferTex.resize(maxLevelNum * 2);

	// 縦横のブラーを作成する
	for (unsigned int i = 0; i < maxLevelNum; i++)
	{
		// 縮小バッファの幅と高さを設定
		w /= 2;
		h /= 2;
		// j = 0 : 横, j = 1 : 縦
		for (unsigned int j = 0; j < 2; j++)
		{
			glGenFramebuffers(1, &m_blurFBO[i * 2 + j]);
			glGenTextures(1, &m_blurBufferTex[i * 2 + j]);
			// FBOとテクスチャをバインド
			glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO[i * 2 + j]);
			glBindTexture(GL_TEXTURE_2D, m_blurBufferTex[i * 2 + j]);

			// テクスチャの作成
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
			// テクスチャフィルタリング・ラッピング設定
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// フレームバッファにテクスチャを関連付け
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurBufferTex[i * 2 + j], 0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}
