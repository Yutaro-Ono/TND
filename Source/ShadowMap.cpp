#include "ShadowMap.h"
#include "Shader.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "MeshComponent.h"

const int ShadowMap::SHADOW_WIDTH = 4096;
const int ShadowMap::SHADOW_HEIGHT = 4096;

// コンストラクタ
ShadowMap::ShadowMap()
{
	glGenFramebuffers(1, &m_depthMapFBO);
	// デプスマップをバインド
	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	// 深度値のみが必要なので、フォーマットはDEPTH_COMPONENT(24bit)に
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// テクスチャラッピング・フィルタリング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// フレームバッファにデプスマップをアタッチする
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	// 光の視点からシーンをレンダリングする際の深度情報のみが必要
	// そのためカラーバッファを使用しないことを明示する
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 画面全体を覆う頂点を定義
	unsigned int vbo;
	float quadVertices[] =
	{
		// x   // y   // z  // u  // v
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	glGenVertexArrays(1, &m_screenVAO);
	glGenBuffers(1, &m_screenVAO);;
	glBindVertexArray(m_screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// シェーダの作成
	m_depthShader = new Shader();
	m_depthShader->Load("Data/Shaders/depthMap.vert", "Data/Shaders/depthMap.frag");
	m_shadowShader = new Shader();
	m_shadowShader->Load("Data/Shaders/ShadowMap.vert", "Data/Shaders/ShadowMap.frag");
}

ShadowMap::~ShadowMap()
{

	glDeleteFramebuffers(1, &m_depthMapFBO);
	glDeleteTextures(1, &m_depthMap);
	glDeleteVertexArrays(1, &m_screenVAO);
	m_depthShader->Delete();
	m_shadowShader->Delete();
}

void ShadowMap::RenderDepthMapFromLightView(Renderer* in_renderer, const std::vector<class MeshComponent*>& in_mesh)
{
	// ライト視点用のプロジェクション行列とビュー行列を用意する
    // ディレクショナルライト(平行)であるため、プロジェクション行列には正射影行列を使用

	// ライト視点の注視点 (プレイヤーの座標を入れる予定)
	Vector3 lightViewTraget = Vector3(1800.0f, 2400.0f, 0.0f);

	m_lightProj = Matrix4::CreateOrtho(static_cast<float>(GAME_CONFIG->GetScreenWidth()), static_cast<float>(GAME_CONFIG->GetScreenHeight()),
		1.0f, 100000.0f);
	m_lightView = Matrix4::CreateLookAt(in_renderer->GetDirectionalLight().m_position, lightViewTraget, Vector3::UnitZ);
	m_lightSpace = m_lightView * m_lightProj;

	// シャドウマップはレンダリング時の解像度とは異なり、シャドウマップのサイズに合わせてViewportパラメータを変更する必要がある
    // そのためglViewportを呼び出す。
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	// フレームバッファのバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_depthShader->SetActive();
	m_depthShader->SetMatrixUniform("lightSpace", m_lightSpace);

	// デプスバッファを得るためにライトから見たシーンをレンダリングする
	//----------------------------------------------------------------------+
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_depthShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

// シャドウとメッシュの描画
void ShadowMap::Draw(const std::vector<class MeshComponent*>& in_mesh)
{
	// ビューポートを元に戻す
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// シャドウシェーダのアクティブ化・uniformへのセット
	m_shadowShader->SetActive();
	m_shadowShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_shadowShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().m_position);
	m_shadowShader->SetMatrixUniform("u_lightSpace", m_lightSpace);
	// サンプリング用テクスチャセット
	m_shadowShader->SetInt("u_mat.diffuseMap", 0);
	m_shadowShader->SetInt("u_mat.specularMao", 1);
	m_shadowShader->SetInt("u_mat.normalMap", 2);
	m_shadowShader->SetInt("u_mat.shadowMap", 3);
	// 3番目にデプスマップをセット
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	
	// メッシュ描画
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_shadowShader);
	}

}
