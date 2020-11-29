//-------------------------------------------------------------------------------------------+
//
// 遅延レンダリングクラス (GBufferに各描画結果を保存・GBufferに対して光源処理を行う)
//
//-------------------------------------------------------------------------------------------+
#include "DefferedRenderer.h"
#include "Shader.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "VertexArray.h"
#include "PointLightComponent.h"
#include "SpriteComponent.h"
#include "WorldSpaceUI.h"
#include "UIScreen.h"
DefferedRenderer::DefferedRenderer(Renderer* in_renderer)
	:m_renderer(in_renderer)
	,m_gBufferShader(nullptr)
	,m_gBufferSkinShader(nullptr)
	,m_screenShader(nullptr)
	,m_pointLightShader(nullptr)
	,m_directionalLightShader(nullptr)
	,m_spotLightShader(nullptr)
{
	std::cout << "CREATE::DefferedRenderer::Instance" << std::endl;
}

DefferedRenderer::~DefferedRenderer()
{
	delete m_gBufferShader;
	delete m_gBufferSkinShader;
	delete m_screenShader;
	delete m_pointLightShader;
	delete m_directionalLightShader;
	delete m_spotLightShader;

}

// 初期化関数
bool DefferedRenderer::Initialize()
{
	// Gバッファとライトバッファの作成
	CreateGBuffer();
	CreateLightBuffer();

	// GBuffer用シェーダの作成
	m_gBufferShader = new Shader();
	if (!m_gBufferShader->Load("Data/Shaders/GBuffer/gBuffer_Basic.vert", "Data/Shaders/GBuffer/gBuffer_Basic.frag"))
	{
		return false;
	}
	// GBuffer用スキンシェーダの作成
	m_gBufferSkinShader = new Shader();
	if (!m_gBufferSkinShader->Load("Data/Shaders/Skinned.vert", "Data/Shaders/GBuffer/gBuffer_Basic.frag"))
	{
		return false;
	}
	// スクリーンシェーダ
	m_screenShader = new Shader();
	if (!m_screenShader->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/FrameBufferScreen.frag"))
	{
		return false;
	}
	// ポイントライトシェーダ
	m_pointLightShader = new Shader();
	if (!m_pointLightShader->Load("Data/Shaders/GBuffer/PointLight.vert", "Data/Shaders/GBuffer/PointLight.frag"))
	{
		return false;
	}
	// ディレクショナルライトシェーダ
	m_directionalLightShader = new Shader();
	if (!m_directionalLightShader->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/FrameBufferScreen.frag"))
	{
		return false;
	}
	// スポットライトシェーダ
	m_spotLightShader = new Shader();
	if (!m_spotLightShader->Load("Data/Shaders/FB/FrameBufferScreen.vert", "Data/Shaders/FB/FrameBufferScreen.frag"))
	{
		return false;
	}

	return true;
}

// GBufferへの書き込み処理
void DefferedRenderer::DrawGBuffer()
{
	// 描画先をGBufferとしてバインドする
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	// ビューポートをスクリーンサイズにセット
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());

	glClearColor(0.0, 0.0, 0.0, 0.0);     // カラーのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 深度テストをON
	glEnable(GL_DEPTH_TEST);

	//-----------------------------------------------------------+
	// 通常メッシュ
	//-----------------------------------------------------------+
	// シェーダにuniformセット
	m_gBufferShader->SetActive();
	m_gBufferShader->SetMatrixUniform("u_view", m_renderer->m_view);
	m_gBufferShader->SetMatrixUniform("u_projection", m_renderer->m_projection);
	m_gBufferShader->SetInt("u_mat.diffuseMap", 0);
	m_gBufferShader->SetInt("u_mat.specularMap", 1);
	// メッシュ描画 (ここでGBufferの各要素に情報が書き込まれる)
	for (auto mesh : m_renderer->m_meshComponents)
	{
		mesh->Draw(m_gBufferShader);
	}

	//------------------------------------------------------------+
	// スキンメッシュ
	//------------------------------------------------------------+
	// シェーダにuniformセット
	m_gBufferSkinShader->SetActive();
	m_gBufferSkinShader->SetMatrixUniform("u_view", m_renderer->m_view);
	m_gBufferSkinShader->SetMatrixUniform("u_projection", m_renderer->m_projection);
	m_gBufferSkinShader->SetInt("u_mat.diffuseMap", 0);
	m_gBufferSkinShader->SetInt("u_mat.specularMap", 1);
	// メッシュ描画 (ここでGBufferの各要素に情報が書き込まれる)
	for (auto skel : m_renderer->m_skeletalMeshComponents)
	{
		skel->Draw(m_gBufferSkinShader);
	}


	// GBufferのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// GBufferを元にライティング計算を行う
void DefferedRenderer::DrawLightPass()
{
	// ライトバッファをバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);
	glEnablei(GL_BLEND, 0);                                          // 加算合成を行うためブレンドを有効化
	glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);         // 加算合成のブレンドを指定
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// 深度テストをオフ
	glDisable(GL_DEPTH_TEST);
	// カリング設定：ライトはメッシュの裏側のみ描画する
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	//------------------------------------------------------+
	// ポイントライト
	//------------------------------------------------------+
	// ポイントライトシェーダへのセット
	m_pointLightShader->SetActive();
	m_pointLightShader->SetMatrixUniform("u_view", m_renderer->GetViewMatrix());
	m_pointLightShader->SetMatrixUniform("u_projection", m_renderer->GetProjectionMatrix());
	m_pointLightShader->SetVectorUniform("u_viewPos", m_renderer->GetViewMatrix().GetTranslation());
	m_pointLightShader->SetInt("u_gPos", 0);
	m_pointLightShader->SetInt("u_gNormal", 1);
	m_pointLightShader->SetInt("u_gAlbedoSpec", 2);
	// gBufferの各テクスチャをバインドしておく
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);

	// ポイントライトの描画
	for (auto pl : m_renderer->m_pointLights)
	{
		pl->Draw(m_pointLightShader);
	}

	// カリングとブレンドを停止する
	glDisable(GL_CULL_FACE);
	glDisablei(GL_BLEND, 0);

	// gBufferの内容をライトバッファへコピーし、gBufferの深度値に基づいた描画を行う
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);       // gBufferを読み取りフレームバッファとして指定
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightFBO);             // 書き込みバッファをライトバッファに指定
	// 深度情報をスクリーン側の深度バッファへコピーする
	glBlitFramebuffer(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	// ライトバッファ描画へ戻す
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);
	
}

// 本描画処理
void DefferedRenderer::Draw()
{

	// GBufferへの書き込み
	DrawGBuffer();

	// ライトパス
	DrawLightPass();

	// ライトFBOへの書き込みを止める
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// GBufferに書き込まれた要素をスクリーンに描画
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	m_screenShader->SetActive();
	m_screenShader->SetInt("u_screenTexture", 0);

	// GBufferテクスチャセット
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_lightHDR);

	// スクリーンに描画
	m_renderer->m_screenVerts->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);



	// ワールド空間上のスプライト描画
	m_renderer->m_worldSpaceSpriteShader->SetActive();
	m_renderer->m_worldSpaceSpriteShader->SetMatrixUniform("u_View", m_renderer->m_view);
	m_renderer->m_worldSpaceSpriteShader->SetMatrixUniform("u_Projection", m_renderer->m_projection);
	for (auto spr : m_renderer->m_worldSprites)
	{
		spr->Draw(m_renderer->m_worldSpaceSpriteShader);
	}

	// Spriteの描画
	// ブレンドのアクティブ化
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 深度テストの停止
	glDisable(GL_DEPTH_TEST);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// spriteシェーダーのアクティブ化
	m_renderer->m_spriteVerts->SetActive();
	m_renderer->m_spriteShader->SetActive();

	for (auto sprite : m_renderer->m_spriteComponents)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(m_renderer->m_spriteShader);
		}
	}

	// 全てのUIを更新
	for (auto ui : GAME_INSTANCE.GetUIStack())
	{
		ui->Draw(m_renderer->m_spriteShader);
	}
}




// GBufferを作成する
// 座標用バッファ・法線用バッファ・アルベド＆スペキュラバッファ・レンダーバッファ
bool DefferedRenderer::CreateGBuffer()
{
	// GBufferの登録・バインド
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	//----------------------------------------------------------------+
	// 各バッファを登録し、2DテクスチャとしてGBufferに紐付ける
	//----------------------------------------------------------------+
	// 3D空間座標バッファ (浮動小数点バッファ/カラー0番目として登録)
	glGenTextures(1, &m_gPos);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPos, 0);
	// 法線ベクトルバッファ (浮動小数点バッファ/カラー1番目として登録)
	glGenFramebuffers(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// アルベド(RGB)＆スペキュラ(A)用カラーバッファ (A成分含む8bitバッファ)
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);

	// 各テクスチャをGBufferの描画先としてGL側に明示する
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// レンダーバッファの作成 (ステンシルバッファとして定義)
	glGenRenderbuffers(1, &m_gRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gRBO);

	// フレームバッファの整合性をチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBUFFER::Create False" << std::endl;
		return false;
	}
	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

// ライトバッファを作成する
// 光源処理用のHDRバッファ・レンダーバッファ
bool DefferedRenderer::CreateLightBuffer()
{
	glGenFramebuffers(1, &m_lightFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// HDRバッファの作成
	glGenTextures(1, &m_lightHDR);
	glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightHDR, 0);
	// HDRテクスチャを描画先0番目に設定
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// レンダーバッファを作成する
	glGenRenderbuffers(1, &m_lightRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_lightRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_lightRBO);

	// フレームバッファの整合性をチェック
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBUFFER::Create False" << std::endl;
		return false;
	}
	// フレームバッファのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return true;
}

