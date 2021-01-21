#include "MiniMapHUD.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"
#include "GameWorld.h"
#include "PlayerManager.h"

MiniMapHUD::MiniMapHUD(Actor* in_target)
	:m_target(in_target)
	,m_miniMapFBO(0)
	,m_mapBuffer(0)
	,m_rbo(0)
	,m_width(300)
	,m_height(300)
	,m_scale(1.0f)
	,m_screenPos(Vector2::Zero)
{
	// ターゲットアクタの座標を取得し、マップ描画のために高所へ設定
	m_viewPos = m_target->GetPosition() + Vector3(0.0f, 0.0f, 500.0f);
	Matrix4 view = Matrix4::CreateLookAt(m_viewPos, m_target->GetPosition(), Vector3::UnitX);
	CreateFBO(m_miniMapFBO);
	RENDERER->SetMapHUD(this);
}

MiniMapHUD::~MiniMapHUD()
{
}

// モデルをマップバッファに書き込む
void MiniMapHUD::WriteBuffer(Shader* in_shader, std::vector<class MeshComponent*> in_mesh)
{
	// フレームバッファのバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_miniMapFBO);

	// バッファに書き込む
	for (auto mesh : in_mesh)
	{
		mesh->Draw(in_shader);
	}

	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// 書き込まれたバッファを描画する
void MiniMapHUD::Draw(Shader* in_shader)
{
	// テクスチャの縦横サイズにスケールを掛け合わせた値をスケール行列として定義
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(m_width * m_scale),
		static_cast<float>(m_height * m_scale),
		1.0f);
	// 指定した画面位置へのスクリーン変換行列を作成
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(m_screenPos.x, m_screenPos.y, 0.0f));
	// スケールと変換行列をワールド行列へ変換
	Matrix4 world = scaleMat * transMat;

	in_shader->SetActive();
	in_shader->SetMatrixUniform("u_worldTranform", world);
	in_shader->SetInt("u_Texture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mapBuffer);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_mapBuffer, 0);

	// バッファをマップFBOの描画先としてGL側に明示する
	unsigned int attachment = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, &attachment);

	// レンダーバッファの作成 (ステンシルバッファとして定義)
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	// フレームバッファの整合性をチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBUFFER::Create False" << std::endl;
	}

	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
