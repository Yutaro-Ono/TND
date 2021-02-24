//-------------------------------------------------------------------------------------------+
//
// 遅延レンダリングクラス (GBufferに各描画結果を保存・GBufferに対して光源処理を行う)
//
//-------------------------------------------------------------------------------------------+
#include "DefferedRenderer.h"
#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>
#include "Shader.h"
#include "Renderer.h"
#include "GameMain.h"
#include "GameConfig.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "EnvironmentMapComponent.h"
#include "CubeMapComponent.h"
#include "VertexArray.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "SpriteComponent.h"
#include "WorldSpaceUI.h"
#include "UIScreen.h"
#include "RenderBloom.h"
#include "FrameBuffer.h"
#include "ParticleManager.h"
#include "CarMeshComponent.h"
#include "CameraComponent.h"
#include "LightGlassComponent.h"
#include "MiniMapHUD.h"

// コンストラクタ
DefferedRenderer::DefferedRenderer(Renderer* in_renderer)
	:m_renderer(in_renderer)
	,m_simpleMeshShader(nullptr)
	,m_meshShader(nullptr)
	,m_skinShader(nullptr)
	,m_skyBoxShader(nullptr)
    ,m_envShader(nullptr)
    ,m_carShader(nullptr)
    ,m_lightGlassShader(nullptr)
	,m_screenShader(nullptr)
	,m_pointLightShader(nullptr)
	,m_directionalLightShader(nullptr)
	,m_spotLightShader(nullptr)
	,m_gBuffer(0)
	,m_gPos(0)
	,m_gNormal(0)
	,m_gAlbedoSpec(0)
	,m_gEmissive(0)
	,m_lightHDR(0)
	,m_uboGBuffer(0)
{
	std::cout << "CREATE::DefferedRenderer::Instance" << std::endl;
}

// デストラクタ
DefferedRenderer::~DefferedRenderer()
{
	delete m_simpleMeshShader;
	delete m_meshShader;
	delete m_skinShader;
	delete m_envShader;
	delete m_carShader;
	delete m_skyBoxShader;
	delete m_screenShader;
	delete m_pointLightShader;
	delete m_directionalLightShader;
	delete m_spotLightShader;
}

// GBufferへの書き込み処理
void DefferedRenderer::DrawGBuffer()
{
	// マップHUD書き込み処理
	if (m_renderer->GetMapHUD() != nullptr)
	{
		m_renderer->GetMapHUD()->WriteBuffer(m_renderer->m_mapInputShader, m_renderer->m_meshComponents);
	}
	// 描画先をGBufferとしてバインドする
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	// ビューポートをスクリーンサイズにセット
	glViewport(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight());
	glClearColor(0.0, 0.0, 0.0, 0.0);     // カラーのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 深度テストをON
	glEnable(GL_DEPTH_TEST);

	// ライト空間の各行列を定義
	Matrix4 lightSpace, lightView, lightProj;
	lightProj = Matrix4::CreateOrtho(7000.0f, 7000.0f, 1.0f, 5000.0f);
	lightView = Matrix4::CreateLookAt(RENDERER->GetDirectionalLight().position, RENDERER->GetDirectionalLight().target, Vector3::UnitZ);
	lightSpace = lightView * lightProj;

	//-----------------------------------------------------------+
	// 通常メッシュ
	//-----------------------------------------------------------+
	// シェーダにuniformセット
	m_meshShader->SetActive();
	m_meshShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_meshShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_meshShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_meshShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);
	m_meshShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_meshShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);
	m_meshShader->SetInt("u_mat.diffuseMap", 0);
	m_meshShader->SetInt("u_mat.specularMap", 1);
	m_meshShader->SetInt("u_mat.normalMap", 2);
	m_meshShader->SetInt("u_mat.emissiveMap", 3);
	m_meshShader->SetInt("u_mat.depthMap", 4);
	// メッシュ描画 (ここでGBufferの各要素に情報が書き込まれる)
	for (auto mesh : m_renderer->m_meshComponents)
	{
		mesh->Draw(m_meshShader);
	}

	
	//------------------------------------------------------------+
	// スキンメッシュ
	//------------------------------------------------------------+
	// シェーダにuniformセット
	m_skinShader->SetActive();
	m_skinShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_skinShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_skinShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_skinShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);
	m_skinShader->SetVectorUniform("u_viewPos", m_renderer->m_view.GetTranslation());
	m_skinShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_skinShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);
	m_skinShader->SetInt("u_mat.diffuseMap", 0);
	m_skinShader->SetInt("u_mat.specularMap", 1);
	m_skinShader->SetInt("u_mat.normalMap", 2);
	m_skinShader->SetInt("u_mat.emissiveMap", 3);
	m_skinShader->SetInt("u_mat.depthMap", 4);
	// メッシュ描画 (ここでGBufferの各要素に情報が書き込まれる)
	for (auto skel : m_renderer->m_skeletalMeshComponents)
	{
		skel->Draw(m_skinShader);
	}

	//------------------------------------------------------------+
	// 車
	//------------------------------------------------------------+
	m_carShader->SetActive();
	m_carShader->SetVectorUniform("u_dirLight.direction", RENDERER->GetDirectionalLight().direction);
	m_carShader->SetVectorUniform("u_dirLight.ambient", RENDERER->GetDirectionalLight().ambient);
	m_carShader->SetVectorUniform("u_dirLight.diffuse", RENDERER->GetDirectionalLight().diffuse);
	m_carShader->SetVectorUniform("u_dirLight.specular", RENDERER->GetDirectionalLight().specular);
	m_carShader->SetMatrixUniform("u_lightSpaceMatrix", lightSpace);
	m_carShader->SetVectorUniform("u_lightPos", RENDERER->GetDirectionalLight().position);
	m_carShader->SetInt("u_mat.diffuseMap", 0);
	m_carShader->SetInt("u_mat.specularMap", 1);
	m_carShader->SetInt("u_mat.depthMap", 2);
	// 車メッシュ描画
	for (auto car : m_renderer->m_carMeshComponents)
	{
		car->Draw(m_carShader);
	}

	//------------------------------------------------------------+
    // SkyBox
    //------------------------------------------------------------+
	m_skyBoxShader->SetActive();
	// Uniformに逆行列をセット
	Matrix4 InvView = m_renderer->m_view;
	InvView.Invert();
	m_skyBoxShader->SetMatrixUniform("u_invView", InvView);
	m_skyBoxShader->SetMatrixUniform("u_projection", m_renderer->m_projection);
	m_skyBoxShader->SetInt("u_skybox", 0);
	m_renderer->GetSkyBox()->Draw(m_skyBoxShader);

	//------------------------------------------------------------+
	// EnvironmentMap
	//------------------------------------------------------------+
	// メッシュ裏側のカリング
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// uniformセット
	m_envShader->SetActive();
	m_envShader->SetVectorUniform("u_viewPos", m_renderer->m_view.GetTranslation());
	m_envShader->SetInt("u_skybox", 0);
	for (auto env : m_renderer->m_envMeshComponents)
	{
		env->DrawEnvironmentMap(m_envShader);
	}
	// カリングのオフ
	glDisable(GL_CULL_FACE);

	//------------------------------------------------------------+
    // ライトグラス
    //------------------------------------------------------------+
	m_lightGlassShader->SetActive();
	m_lightGlassShader->SetVectorUniform("u_viewPos", m_renderer->m_view.GetTranslation());
	m_lightGlassShader->SetInt("u_skybox", 0);
	for (auto light : m_renderer->m_lightGlassComponents)
	{
		light->Draw(m_lightGlassShader);
	}

	// GBufferのバインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

// GBufferを元にライティング計算を行う
void DefferedRenderer::DrawLightPass()
{
	// ライトバッファをバインド
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);

	// ブレンド指定
	glEnablei(GL_BLEND, 0);                                          // 加算合成を行うためブレンドを有効化
	glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);         // 加算合成のブレンドを指定
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 深度テストをオフ
	glDisable(GL_DEPTH_TEST);

	// gBufferの各テクスチャをバインド
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);

	// カリング設定：ライトはメッシュの裏側のみ描画する
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	//------------------------------------------------------+
	// ポイントライト
	//------------------------------------------------------+
	// ポイントライトシェーダへのセット
	m_pointLightShader->SetActive();
	m_pointLightShader->SetVectorUniform("u_viewPos",    GAME_INSTANCE.GetViewVector());
	m_pointLightShader->SetInt("u_gBuffer.pos",     0);
	m_pointLightShader->SetInt("u_gBuffer.normal",       1);
	m_pointLightShader->SetInt("u_gBuffer.albedoSpec",   2);
	m_pointLightShader->SetInt("u_gBuffer.emissive", 3);
	// ポイントライトの描画
	for (auto pl : m_renderer->m_pointLights)
	{
		pl->Draw(m_pointLightShader);
	}


	//------------------------------------------------------+
	// スポットライト
	//------------------------------------------------------+
	// スポットライトシェーダへのセット
	m_spotLightShader->SetActive();
	m_spotLightShader->SetMatrixUniform("u_view", m_renderer->GetViewMatrix());
	m_spotLightShader->SetMatrixUniform("u_projection", m_renderer->GetProjectionMatrix());
	m_spotLightShader->SetVectorUniform("u_viewPos", m_renderer->GetViewMatrix().GetTranslation());
	m_spotLightShader->SetInt("u_gBuffer.pos", 0);
	m_spotLightShader->SetInt("u_gBuffer.normal", 1);
	m_spotLightShader->SetInt("u_gBuffer.albedoSpec", 2);
	m_spotLightShader->SetInt("u_gBuffer.emissive", 3);
	// スポットライトの描画
	for (auto spotL : m_renderer->m_spotLights)
	{
		spotL->Draw(m_spotLightShader);
	}

	// カリングのオフ
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);


	//-----------------------------------------------+
	// ディレクショナルライトパス
	//-----------------------------------------------+
	// 輝度定義
	float intensity = 1.65f;
	// シェーダのセット
	m_directionalLightShader->SetActive();
	m_directionalLightShader->SetVectorUniform("u_viewPos", GAME_INSTANCE.GetViewVector());
	m_directionalLightShader->SetVectorUniform("u_dirLight.direction",    m_renderer->m_directionalLight.direction);
	m_directionalLightShader->SetVectorUniform("u_dirLight.ambientColor", m_renderer->m_directionalLight.ambient);
	m_directionalLightShader->SetVectorUniform("u_dirLight.color",        m_renderer->m_directionalLight.diffuse);
	m_directionalLightShader->SetVectorUniform("u_dirLight.specular",     m_renderer->m_directionalLight.specular);
	m_directionalLightShader->SetFloat("u_dirLight.intensity", intensity);
	m_directionalLightShader->SetInt("u_gBuffer.pos", 0);
	m_directionalLightShader->SetInt("u_gBuffer.normal", 1);
	m_directionalLightShader->SetInt("u_gBuffer.albedoSpec", 2);
	m_directionalLightShader->SetInt("u_gBuffer.emissive", 3);
	// スクリーン全体に描画
	m_renderer->GetScreenVAO()->SetActive();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Spriteの描画
	// ブレンドのアクティブ化
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 深度テストの停止
	glDisable(GL_DEPTH_TEST);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);


	// ワールド空間上のスプライト描画
	Matrix4 view = m_renderer->GetViewMatrix();
	Matrix4 projection = m_renderer->GetProjectionMatrix();
	m_bloomWorldSpriteShader->SetActive();
	m_bloomWorldSpriteShader->SetMatrixUniform("u_view", view);
	m_bloomWorldSpriteShader->SetMatrixUniform("u_projection", projection);
	for (auto spr : m_renderer->m_worldSprites)
	{
		spr->Draw(m_bloomWorldSpriteShader);
	}

	// spriteシェーダーのアクティブ化
	m_bloomSpriteShader->SetActive();
	m_bloomSpriteShader->SetFloat("u_intensity", 1.0f);
	RENDERER->SetActiveSpriteVAO();
	for (auto sprite : m_renderer->m_spriteComponents)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(m_bloomSpriteShader);
		}
	}
	// 全てのUIを更新
	for (auto ui : GAME_INSTANCE.GetUIStack())
	{
		ui->Draw(m_bloomSpriteShader);
	}

	// マップHUD
	if (m_renderer->GetMapHUD() != nullptr)
	{
		m_renderer->GetMapHUD()->Draw(m_renderer->m_mapOutputShader);
	}

	// ブレンドをオフ
	glDisable(GL_BLEND);

	// ブレンドを停止する
	glDisablei(GL_BLEND, 0);

	// gBufferの深度情報をライトバッファへコピーする
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);              // gBufferを読み取りフレームバッファとして指定
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_lightFBO);             // 書き込みバッファをライトバッファに指定
	glBlitFramebuffer(0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, 0, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	// ライトバッファ描画へ戻す
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFBO);


	// 深度テストをオン
	glEnable(GL_DEPTH_TEST);
	
	//----------------------------------------------------------------+
    // パーティクル描画
    //----------------------------------------------------------------+
	m_renderer->GetParticleManager()->Draw();


	// ライトFBOへの書き込みを止める
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// 本描画処理
void DefferedRenderer::Draw()
{

	// GBufferへの書き込み
	DrawGBuffer();

	// ライトバッファへの書き込み
	DrawLightPass();

	// Bloom処理を施した描画
	RenderBloom* bloom = m_renderer->GetBloom();
	//bloom->SetExposureVal(4.5f);
	bloom->DrawDownSampling(m_lightHighBright);
	bloom->DrawGaussBlur();
	bloom->DrawBlendBloom(m_lightHDR);

	//----------------------------------------------------------------+
	// 最終出力結果を描画
	//----------------------------------------------------------------+
	// GBufferに書き込まれた要素をスクリーンに描画
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	//// スクリーンシェーダのuniformセット
	//m_screenShader->SetActive();
	//m_screenShader->SetInt("u_screenTexture", 0);

	//// GBufferテクスチャセット
	//glActiveTexture(GL_TEXTURE0);
	////glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	////glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	//glBindTexture(GL_TEXTURE_2D, m_gEmissive);
	////glBindTexture(GL_TEXTURE_2D, m_gBrightBuffer);

	//// スクリーンに描画
	//m_renderer->m_screenVerts->SetActive();
	//glDrawArrays(GL_TRIANGLES, 0, 6);

}

void DefferedRenderer::LinkUniformBuffers()
{
	m_meshShader->LinkUniformBuffer();
	m_skinShader->LinkUniformBuffer();
	m_carShader->LinkUniformBuffer();
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
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// アルベド(RGB)＆スペキュラ(A)用カラーバッファ (A成分含む8bitカラーバッファ/2番目として登録)
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);
	// 高輝度バッファの作成 (エミッシブ出力用輝度バッファ/3番目として登録)
	glGenTextures(1, &m_gEmissive);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gEmissive, 0);


	// 各テクスチャをGBufferの描画先としてGL側に明示する
	m_gAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_gAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	m_gAttachments[2] = { GL_COLOR_ATTACHMENT2 };
	m_gAttachments[3] = { GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, m_gAttachments);

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
	// 高輝度バッファの作成
	glGenTextures(1, &m_lightHighBright);
	glBindTexture(GL_TEXTURE_2D, m_lightHighBright);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG->GetScreenWidth(), GAME_CONFIG->GetScreenHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightHighBright, 0);
	// アタッチメント設定
	m_lightAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_lightAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_lightAttachments);

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


/// <summary> 
/// Defferedレンダリングに用いる変数の初期化・インスタンスの生成
/// </summary>
/// <returns></returns>
bool DefferedRenderer::Initialize()
{
	// Gバッファとライトバッファの作成
	CreateGBuffer();
	CreateLightBuffer();

	//--------------------------------------------------------------------------------------------------------------------+
	// GBuffer出力用シェーダ
	//--------------------------------------------------------------------------------------------------------------------+
	// シンプルなメッシュ用シェーダ
	m_simpleMeshShader = new Shader();
	if (!m_simpleMeshShader->Load("Data/Shaders/BasicMesh.vert", "Data/Shaders/BasicMesh.frag"))
	{
		return false;
	}
	// GBuffer用メッシュシェーダの作成
	m_meshShader = new Shader();
	if (!m_meshShader->Load("Data/Shaders/GBuffer/gBuffer_NormShadow.vert", "Data/Shaders/GBuffer/gBuffer_NormShadow.frag"))
	{
		return false;
	}

	// GBuffer用スキンシェーダの作成
	m_skinShader = new Shader();
	if (!m_skinShader->Load("Data/Shaders/GBuffer/gBuffer_SkinNormShadow.vert", "Data/Shaders/GBuffer/gBuffer_Shadow.frag"))
	{
		return false;
	}

	// GBuffer用スカイボックスシェーダ
	m_skyBoxShader = new Shader();
	if (!m_skyBoxShader->Load("Data/Shaders/GBuffer/gBuffer_SkyBox.vert", "Data/Shaders/GBuffer/gBuffer_SkyBox.frag"))
	{
		return false;
	}
	// GBuffer用環境マップシェーダ
	m_envShader = new Shader();
	if (!m_envShader->Load("Data/Shaders/GBuffer/gBuffer_EnvironmentMap.vert", "Data/Shaders/GBuffer/gBuffer_EnvironmentMap.frag"))
	{
		return false;
	}
	// GBuffer車用シェーダ
	m_carShader = new Shader();
	if (!m_carShader->Load("Data/Shaders/GBuffer/gBuffer_CarShaderReflect.vert", "Data/Shaders/GBuffer/gBuffer_CarShaderReflect.frag"))
	{
		return false;
	}

	// GBufferライトグラスシェーダ
	m_lightGlassShader = new Shader();
	if (!m_lightGlassShader->Load("Data/Shaders/GBuffer/GBuffer_LightGlass.vert", "Data/Shaders/GBuffer/GBuffer_LightGlass.frag"))
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
	if (!m_pointLightShader->Load("Data/Shaders/GBuffer/PointLight.vert", "Data/Shaders/GBuffer/PointLight_Bloom.frag"))
	{
		return false;
	}
	// ディレクショナルライトシェーダ
	m_directionalLightShader = new Shader();
	if (!m_directionalLightShader->Load("Data/Shaders/GBuffer/DirectionalLight.vert", "Data/Shaders/GBuffer/DirectionalLight_Bloom.frag"))
	{
		return false;
	}
	// スポットライトシェーダ
	m_spotLightShader = new Shader();
	if (!m_spotLightShader->Load("Data/Shaders/GBuffer/SpotLight_Bloom.vert", "Data/Shaders/GBuffer/SpotLight_Bloom.frag"))
	{
		return false;
	}

	// スプライトシェーダ
	m_bloomSpriteShader = new Shader();
	if (!m_bloomSpriteShader->Load("Data/Shaders/SpriteShader.vert", "Data/Shaders/GBuffer/SpriteShader_Bloom.frag"))
	{
		return false;
	}
	m_bloomSpriteShader->SetActive();
	// スクリーン用の行列を作成 (UIやスプライトは以降この行列を基準に描画)
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(m_renderer->m_screenWidth, m_renderer->m_screenHeight);
	m_bloomSpriteShader->SetMatrixUniform("u_viewProj", viewProj);

	// ワールドスプライトシェーダ
	m_bloomWorldSpriteShader = new Shader();
	if (!m_bloomWorldSpriteShader->Load("Data/Shaders/GBuffer/Particle_Bloom.vert", "Data/Shaders/GBuffer/WorldSpaceSprite_Bloom.frag"))
	{
		return false;
	}


	return true;
}