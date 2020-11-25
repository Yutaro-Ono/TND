#include "ForwardRenderer.h"
#include "Renderer.h"

// コンストラクタ
ForwardRenderer::ForwardRenderer(Renderer* in_renderer)
	:m_renderer(in_renderer)
{
}

// デストラクタ
ForwardRenderer::~ForwardRenderer()
{
}

// 描画処理
//void ForwardRenderer::Draw()
//{
//
//	// 深度テストをON、ブレンドをオフ
//	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_BLEND);
//	// 画面をクリア
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	// カラーバッファのクリア
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//	//----------------------------------------------+
//	// メッシュシェーダー(phong)
//	//----------------------------------------------+
//	if (m_switchShader == 2)
//	{
//		// フレームバッファ書き込み処理
//		m_frameBuffer->WriteFrameBuffer();
//
//		//メッシュシェーダーで描画する対象の変数をセット
//		m_meshShader->SetActive();
//		m_meshShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
//		// ライティング変数をセット
//		SetLightUniforms(m_meshShader);
//		// 全てのメッシュコンポーネントを描画
//		for (auto mc : m_meshComponents)
//		{
//			if (mc->GetVisible())
//			{
//				mc->Draw(m_meshShader);
//			}
//		}
//
//		//-----------------------------------------------------------+
//		// ボーン入りメッシュの描画
//		//-----------------------------------------------------------+
//		m_skinnedShader->SetActive();
//		// ビュー・プロジェクションの合成行列をセット
//		m_skinnedShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
//		// ライティング変数をセット
//		SetLightUniforms(m_skinnedShader);
//
//		for (auto sk : m_skeletalMeshComponents)
//		{
//			if (sk->GetVisible())
//			{
//				sk->Draw(m_skinnedShader);
//			}
//		}
//	}
//
//	//-----------------------------------------------+
//	// メッシュシェーダー(normal)
//	//-----------------------------------------------+
//	if (m_switchShader == 1)
//	{
//		// フレームバッファ書き込み処理
//		m_frameBuffer->WriteFrameBuffer();
//
//		//メッシュシェーダーで描画する対象の変数をセット
//		m_meshNormalShader->SetActive();
//		m_meshNormalShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
//		// ライティング変数をセット
//		SetLightUniforms(m_meshNormalShader);
//		m_meshNormalShader->SetVectorUniform("u_lightPos", m_directionalLight.position);
//		m_meshNormalShader->SetVectorUniform("u_viewPos", m_view.GetTranslation());
//		// 全てのメッシュコンポーネントを描画
//		for (auto mc : m_meshComponents)
//		{
//			if (mc->GetVisible())
//			{
//				mc->Draw(m_meshNormalShader);
//			}
//		}
//
//		//-----------------------------------------------------------+
//		// ボーン入りメッシュの描画
//		//-----------------------------------------------------------+
//		m_skinnedShader->SetActive();
//		// ビュー・プロジェクションの合成行列をセット
//		m_skinnedShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
//		// ライティング変数をセット
//		SetLightUniforms(m_skinnedShader);
//
//		for (auto sk : m_skeletalMeshComponents)
//		{
//			if (sk->GetVisible())
//			{
//				sk->Draw(m_skinnedShader);
//			}
//		}
//	}
//
//	//-----------------------------------------------+
//	// メッシュシェーダー(shadow)
//	//-----------------------------------------------+
//	//if (m_switchShader == 0)
//	//{
//	//	m_shadowMap->RenderDepthMapFromLightView(m_meshComponents, m_skeletalMeshComponents);
//
//	//	// フレームバッファ書き込み処理
//	//	m_frameBuffer->WriteFrameBuffer();
//
//	//	m_shadowMap->DrawShadowMesh(m_meshComponents, m_skeletalMeshComponents);
//	//}
//
//
//
//	//-----------------------------------------------+
//	// 環境マップオブジェクトの描画
//	//-----------------------------------------------+
//	// シェーダを有効化・uniformに各行列をセット
//	m_environmentMapShader->SetActive();
//	m_environmentMapShader->SetMatrixUniform("u_viewMat", m_view);
//	m_environmentMapShader->SetMatrixUniform("u_projMat", m_projection);
//	m_environmentMapShader->SetVectorUniform("u_viewPos", m_view.GetTranslation());
//	m_environmentMapShader->SetInt("u_skybox", 0);
//	for (auto env : m_envMeshComponents)
//	{
//		env->DrawEnvironmentMap(m_environmentMapShader);
//	}
//
//
//
//	if (m_switchShader == 0)
//	{
//		m_shadowMap->RenderDepthMapFromLightView(m_meshComponents, m_skeletalMeshComponents);
//
//		// フレームバッファ書き込み処理
//		m_frameBuffer->WriteFrameBuffer();
//
//		m_bloom->WriteBuffer(m_meshComponents, m_skeletalMeshComponents, m_activeSkyBox, m_envMeshComponents);
//		m_bloom->WriteBuffer(m_particleManager);
//
//		m_bloom->DrawDownSampling();
//		m_bloom->DrawGaussBlur();
//		m_bloom->DrawBlendBloom();
//
//	}
//
//
//	//---------------------------------------------------------------+
//	// スカイボックスの描画
//	//---------------------------------------------------------------+
//	// キューブマップシェーダをアクティブ化・キューブVAOをバインド
//	//m_activeSkyBox->Draw(m_skyboxShader);
//
//	//----------------------------------------------------------------+
//	// パーティクル描画
//	//----------------------------------------------------------------+
//	//glEnable(GL_DEPTH_TEST);
//	//m_particleManager->Draw();
//	// ワールド空間上のスプライト描画
//	m_worldSpaceSpriteShader->SetActive();
//	m_worldSpaceSpriteShader->SetMatrixUniform("u_View", m_view);
//	m_worldSpaceSpriteShader->SetMatrixUniform("u_Projection", m_projection);
//	for (auto spr : m_worldSprites)
//	{
//		spr->Draw(m_worldSpaceSpriteShader);
//	}
//	// フレームバッファ描画
//	m_frameBuffer->DrawFrameBuffer();
//
//	// Spriteの描画
//	// ブレンドのアクティブ化
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	// 深度テストの停止
//	glDisable(GL_DEPTH_TEST);
//	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
//
//	// spriteシェーダーのアクティブ化
//	m_spriteVerts->SetActive();
//	m_spriteShader->SetActive();
//
//	for (auto sprite : m_spriteComponents)
//	{
//		if (sprite->GetVisible())
//		{
//			sprite->Draw(m_spriteShader);
//		}
//	}
//
//	// 全てのUIを更新
//	for (auto ui : GAME_INSTANCE.GetUIStack())
//	{
//		ui->Draw(m_spriteShader);
//	}
//
//
//
//
//}
