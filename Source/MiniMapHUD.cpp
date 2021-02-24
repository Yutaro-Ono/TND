#include "MiniMapHUD.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "Texture.h"
#include "MeshComponent.h"
#include "LandMarkIndicator.h"

MiniMapHUD::MiniMapHUD(PlayerManager* in_target)
	:m_target(in_target)
	,m_miniMapFBO(0)
	,m_mapBuffer(0)
	,m_rbo(0)
	,m_width(1024)
	,m_height(512)
	,m_scale(0.7f)
	,m_screenPos(Vector2(-GAME_CONFIG->GetScreenWidth() / 2 + 35.0f, -GAME_CONFIG->GetScreenHeight() / 2 + m_height / 2 + 5.0f))
	,m_mapTex(nullptr)
{
	// ターゲットアクタの座標を取得し、マップ描画のために高所へ設定
	m_viewPos = m_target->GetPosition() + Vector3(0.0f, 0.0f, 4500.0f);

	// プロジェクション行列を定義
	m_projection = Matrix4::CreateOrtho(
		static_cast<float>(m_width) * 15.0f,
		static_cast<float>(m_height) * 15.0f,
		1.0f, 6000.0f);

	CreateFBO(m_miniMapFBO);
	RENDERER->SetMapHUD(this);

	// マップテクスチャを生成
	m_mapTex = new Texture();
	m_mapTex->Load("Data/Interface/HUD/Map/MapHUD.png");

	// HUD上の矢印を生成
	m_landMark = new LandMarkIndicator(in_target);
}

MiniMapHUD::~MiniMapHUD()
{
	
}

// モデルをマップバッファに書き込む
void MiniMapHUD::WriteBuffer(Shader* in_shader, std::vector<class MeshComponent*> in_mesh)
{

	// フレームバッファのバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_miniMapFBO);
	// ビューポートを変更
	glViewport(0, 0, m_width, m_height);
	glClearColor(0.0, 0.0, 0.0, 0.0);     // カラーのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 深度テストをON
	glEnable(GL_DEPTH_TEST);

	// ターゲットアクタの座標を取得し、マップ描画のために高所へ設定
	m_viewPos = m_target->GetPosition() + Vector3(0.0f, -3900.0f, 4500.0f);
	Matrix4 view = Matrix4::CreateLookAt(m_viewPos, m_target->GetPosition() + Vector3(0.0f, -3900.0f, 0.0f), Vector3::UnitX);
	// ビュー・プロジェクション合成行列を作成
	Matrix4 viewProj = view * m_projection;
	
	in_shader->SetActive();
	in_shader->SetMatrixUniform("u_viewProj", viewProj);
	glClearColor(1.0f, 0.6f, 0.0f, 1.0f);

	// バッファに書き込む
	for (auto mesh : in_mesh)
	{
		mesh->DrawMap(in_shader);
	}

	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//// ビューポートを画面サイズに戻す
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());

}

// 書き込まれたバッファを描画する
void MiniMapHUD::Draw(Shader* in_shader)
{
	// テクスチャの縦横サイズにスケールを掛け合わせた値をスケール行列として定義
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(m_width) * m_scale,
		static_cast<float>(m_height) * m_scale,
		1.0f);
	// 指定した画面位置へのスクリーン変換行列を作成
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
	// 回転行列
	Matrix4 rotZ = Matrix4::CreateRotationZ(Math::ToRadians(180.0f));
	Matrix4 rotY = Matrix4::CreateRotationY(Math::ToRadians(180.0f));
	// スケールと変換行列をワールド行列へ変換
	Matrix4 world = scaleMat * rotZ * rotY * transMat;

	in_shader->SetActive();
	in_shader->SetMatrixUniform("u_worldTransform", world);
	//in_shader->SetMatrixUniform("u_viewProj", viewProj);
	in_shader->SetInt("u_texture", 0);
	in_shader->SetInt("u_mapTex", 1);
	in_shader->SetFloat("u_intensity", 0.1f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mapBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_mapTex->GetTextureID());


	RENDERER->SetActiveSpriteVAO();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

// ミニマップ用のフレームバッファ作成
void MiniMapHUD::CreateFBO(unsigned int& in_fbo)
{
	glGenFramebuffers(1, &in_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, in_fbo);

	// マップ画面描画用バッファ
	glGenTextures(1, &m_mapBuffer);
	glBindTexture(GL_TEXTURE_2D, m_mapBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_mapBuffer, 0);

	// バッファをマップFBOの描画先としてGL側に明示する
	unsigned int attachment = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &attachment);

	// レンダーバッファの作成 (ステンシルバッファとして定義)
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	// フレームバッファの整合性をチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBUFFER::Create False" << std::endl;
	}

	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
