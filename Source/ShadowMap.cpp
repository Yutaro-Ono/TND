#include "ShadowMap.h"
#include "Shader.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "CameraComponent.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"

const int ShadowMap::SHADOW_WIDTH = 8192;
const int ShadowMap::SHADOW_HEIGHT = 8192;

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


	// シェーダの作成
	m_depthShader = new Shader();
	m_depthShader->Load("Data/Shaders/DepthMap.vert", "Data/Shaders/DepthMap.frag");
	//m_shadowShader = new Shader();
	//m_shadowShader->Load("Data/Shaders/PhongShadow.vert", "Data/Shaders/PhongShadow.frag");
	m_shadowShader = new Shader();
	m_shadowShader->Load("Data/Shaders/ShadowNormalMap.vert", "Data/Shaders/ShadowNormalMap.frag");
	
	//m_depthSkinShader = new Shader();
	//m_depthSkinShader->Load("Data/Shaders/SkinnedDepth.vert", "Data/Shaders/DepthMap.frag");
	//m_skinShadowShader = new Shader();
	//m_skinShadowShader->Load("Data/Shaders/SkinnedShadow.vert", "Data/Shaders/PhongShadow.frag");
	
	m_depthSkinShader = new Shader();
	m_depthSkinShader->Load("Data/Shaders/SkinnedDepthNormal.vert", "Data/Shaders/DepthMap.frag");
	m_skinShadowShader = new Shader();
	m_skinShadowShader->Load("Data/Shaders/ShadowSkinnedNormal.vert", "Data/Shaders/ShadowNormalMap.frag");

}

ShadowMap::~ShadowMap()
{

	glDeleteFramebuffers(1, &m_depthMapFBO);
	glDeleteTextures(1, &m_depthMap);
	//glDeleteVertexArrays(1, &m_screenVAO);
	m_depthShader->Delete();
	m_depthSkinShader->Delete();
	m_shadowShader->Delete();
	m_skinShadowShader->Delete();
}

void ShadowMap::RenderDepthMapFromLightView(Renderer* in_renderer, const std::vector<class MeshComponent*>& in_mesh)
{
	// 深度テスト有効化
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ライト視点用のプロジェクション行列とビュー行列を用意する
    // ディレクショナルライト(平行)であるため、プロジェクション行列には正射影行列を使用

	// ライト視点の注視点 (プレイヤーの座標を入れる予定)
	Vector3 lightViewTraget = Vector3(12000.0f, 14000.0f, -12.0f);
	Vector3 direction = lightViewTraget - in_renderer->GetDirectionalLight().position;
	direction.Normalize();

	m_lightProj = Matrix4::CreateOrtho(10000.0f, 10000.0f, 1.0f, 10000.0f);
	m_lightView = Matrix4::CreateLookAt(in_renderer->GetDirectionalLight().position, lightViewTraget, Vector3::UnitZ);
	m_lightSpace = m_lightView * m_lightProj;

	// シャドウマップはレンダリング時の解像度とは異なり、シャドウマップのサイズに合わせてViewportパラメータを変更する必要がある
    // そのためglViewportを呼び出す。
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	// フレームバッファのバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_depthShader->SetActive();
	m_depthShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);

	// デプスバッファを得るためにライトから見たシーンをレンダリングする
	//----------------------------------------------------------------------+
	for (auto mesh : in_mesh)
	{
		mesh->DrawShadow(m_depthShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ビューポートを元に戻す
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// シャドウシェーダのアクティブ化・uniformへのセット
	m_shadowShader->SetActive();
	m_shadowShader->SetMatrixUniform("u_view", RENDERER->GetViewMatrix());
	m_shadowShader->SetMatrixUniform("u_projection", RENDERER->GetProjectionMatrix());
	m_shadowShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);
	m_shadowShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	m_shadowShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_shadowShader->SetVectorUniform("u_dirLight.direction", direction);
	m_shadowShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_shadowShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_shadowShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

}

// 
void ShadowMap::RenderDepthMapFromLightView(const std::vector<class MeshComponent*>& in_mesh, const std::vector<class SkeletalMeshComponent*> in_skelMesh)
{
	// 深度テスト有効化
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ライト視点用のプロジェクション行列とビュー行列を用意する
	// ディレクショナルライト(平行)であるため、プロジェクション行列には正射影行列を使用

	// ライト視点の注視点 (プレイヤーの座標を入れる予定)
	//Vector3 lightViewTraget = Vector3(12000.0f, 14000.0f, -12.0f);

	//Vector3 direction = lightViewTraget - RENDERER->GetDirectionalLight().position;
	//direction.Normalize();

	Vector3 direction = RENDERER->GetDirectionalLight().direction;

	m_lightProj = Matrix4::CreateOrtho(8000.0f, 8000.0f, 1.0f, 8000.0f);
	//m_lightProj = Matrix4::CreateOrtho(35000.0f, 20000.0f, 1.0f, 35000.0f);

	//m_lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight().position, lightViewTraget, Vector3::UnitZ);
	m_lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight().position, RENDERER->GetDirectionalLight().target, Vector3::UnitZ);

	m_lightSpace = m_lightView * m_lightProj;

	// シャドウマップはレンダリング時の解像度とは異なり、シャドウマップのサイズに合わせてViewportパラメータを変更する必要がある
	// そのためglViewportを呼び出す。
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	// フレームバッファのバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_depthShader->SetActive();
	m_depthShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);

	// デプスバッファを得るためにライトから見たシーンをレンダリングする
	//----------------------------------------------------------------------+
	for (auto mesh : in_mesh)
	{
		mesh->DrawShadow(m_depthShader);
	}

	m_depthSkinShader->SetActive();
	m_depthSkinShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);
	for (auto skel : in_skelMesh)
	{
		if (skel->GetVisible())
		{
			skel->DrawShadow(m_depthSkinShader);
		}

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ビューポートを元に戻す
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}



// シャドウとメッシュの描画 (スキンメッシュは対象外)
void ShadowMap::DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh)
{
	// サンプリング用テクスチャセット
	m_shadowShader->SetInt("u_mat.diffuseMap", 0);
	m_shadowShader->SetInt("u_mat.specularMap", 1);
	m_shadowShader->SetInt("u_mat.normalMap", 2);
	m_shadowShader->SetInt("u_mat.depthMap", 3);
	
	// シャドウシェーダによるメッシュ描画
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_shadowShader);
	}

}

void ShadowMap::DrawShadowMesh(const std::vector<class MeshComponent*>& in_mesh, const std::vector<class SkeletalMeshComponent*>& in_skelMesh)
{

	// シャドウシェーダのアクティブ化・uniformへのセット
	m_shadowShader->SetActive();
	m_shadowShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_shadowShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_shadowShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_shadowShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_shadowShader->SetMatrixUniform("u_view", RENDERER->GetViewMatrix());
	m_shadowShader->SetMatrixUniform("u_projection", RENDERER->GetProjectionMatrix());
	m_shadowShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);
	m_shadowShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_shadowShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);

	// サンプリング用テクスチャセット
	m_shadowShader->SetInt("u_mat.diffuseMap", 0);
	m_shadowShader->SetInt("u_mat.specularMap", 1);
	m_shadowShader->SetInt("u_mat.normalMap", 2);
	m_shadowShader->SetInt("u_mat.depthMap", 3);

	// シャドウシェーダによるメッシュ描画
	for (auto mesh : in_mesh)
	{
		mesh->Draw(m_shadowShader);
	}


	// シャドウシェーダのアクティブ化・uniformへのセット
	m_skinShadowShader->SetActive();
	m_skinShadowShader->SetVectorUniform("u_viewPos", RENDERER->GetViewMatrix().GetTranslation());
	m_skinShadowShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_skinShadowShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_skinShadowShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_skinShadowShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);

	m_skinShadowShader->SetMatrixUniform("u_view", RENDERER->GetViewMatrix());
	m_skinShadowShader->SetMatrixUniform("u_projection", RENDERER->GetProjectionMatrix());
	m_skinShadowShader->SetMatrixUniform("u_lightSpaceMatrix", m_lightSpace);
	m_skinShadowShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);


	// サンプリング用テクスチャセット
	m_skinShadowShader->SetInt("u_mat.diffuseMap", 0);
	m_skinShadowShader->SetInt("u_mat.specularMap", 1);
	m_skinShadowShader->SetInt("u_mat.normalMap", 2);
	m_skinShadowShader->SetInt("u_mat.depthMap", 3);
	// シャドウシェーダによるスキンメッシュ描画
	for (auto skel : in_skelMesh)
	{
		if (skel->GetVisible())
		{
			skel->Draw(m_skinShadowShader);
		}

	}

}
