#include "ForwardRenderer.h"
#include "Renderer.h"
#include "GameMain.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "SpriteComponent.h"
#include "EnvironmentMapComponent.h"
#include "ShadowMap.h"
#include "RenderBloom.h"
#include "WorldSpaceUI.h"
#include "VertexArray.h"
#include "UIScreen.h"

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
void ForwardRenderer::Draw()
{

	// 深度テストをON、ブレンドをオフ
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// 画面をクリア
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// カラーバッファのクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ビュー行列・プロジェクション行列をレンダラーから取得
	Matrix4 view = m_renderer->m_view;
	Matrix4 proj = m_renderer->m_projection;

	//----------------------------------------------+
	// メッシュシェーダー(phong)
	//----------------------------------------------+
	if (m_renderer->m_switchShader == 2)
	{
		// フレームバッファ書き込み処理
		m_renderer->m_frameBuffer->WriteFrameBuffer();

		//メッシュシェーダーで描画する対象の変数をセット
		m_renderer->m_meshShader->SetActive();
		m_renderer->m_meshShader->SetMatrixUniform("u_viewProj", view * proj);
		// ライティング変数をセット
		m_renderer->SetLightUniforms(m_renderer->m_meshShader);
		// 全てのメッシュコンポーネントを描画
		for (auto mc : m_renderer->m_meshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(m_renderer->m_meshShader);
			}
		}

		//-----------------------------------------------------------+
		// ボーン入りメッシュの描画
		//-----------------------------------------------------------+
		m_renderer->m_skinnedShader->SetActive();
		// ビュー・プロジェクションの合成行列をセット
		m_renderer->m_skinnedShader->SetMatrixUniform("u_viewProj", view * proj);
		// ライティング変数をセット
		m_renderer->SetLightUniforms(m_renderer->m_skinnedShader);

		for (auto sk : m_renderer->m_skeletalMeshComponents)
		{
			if (sk->GetVisible())
			{
				sk->Draw(m_renderer->m_skinnedShader);
			}
		}
	}

	//-----------------------------------------------+
	// メッシュシェーダー(normal)
	//-----------------------------------------------+
	if (m_renderer->m_switchShader == 1)
	{
		// フレームバッファ書き込み処理
		m_renderer->m_frameBuffer->WriteFrameBuffer();

		//メッシュシェーダーで描画する対象の変数をセット
		m_renderer->m_meshNormalShader->SetActive();
		m_renderer->m_meshNormalShader->SetMatrixUniform("u_viewProj", view * proj);
		// ライティング変数をセット
		m_renderer->SetLightUniforms(m_renderer->m_meshNormalShader);
		m_renderer->m_meshNormalShader->SetVectorUniform("u_lightPos", m_renderer->m_directionalLight.position);
		m_renderer->m_meshNormalShader->SetVectorUniform("u_viewPos", view.GetTranslation());
		// 全てのメッシュコンポーネントを描画
		for (auto mc : m_renderer->m_meshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(m_renderer->m_meshNormalShader);
			}
		}

		//-----------------------------------------------------------+
		// ボーン入りメッシュの描画
		//-----------------------------------------------------------+
		m_renderer->m_skinnedShader->SetActive();
		// ビュー・プロジェクションの合成行列をセット
		m_renderer->m_skinnedShader->SetMatrixUniform("u_viewProj", view * proj);
		// ライティング変数をセット
		m_renderer->SetLightUniforms(m_renderer->m_skinnedShader);

		for (auto sk : m_renderer->m_skeletalMeshComponents)
		{
			if (sk->GetVisible())
			{
				sk->Draw(m_renderer->m_skinnedShader);
			}
		}
	}

	//-----------------------------------------------+
	// メッシュシェーダー(shadow)
	//-----------------------------------------------+
	//if (m_switchShader == 0)
	//{
	//	m_shadowMap->RenderDepthMapFromLightView(m_meshComponents, m_skeletalMeshComponents);

	//	// フレームバッファ書き込み処理
	//	m_frameBuffer->WriteFrameBuffer();

	//	m_shadowMap->DrawShadowMesh(m_meshComponents, m_skeletalMeshComponents);
	//}



	//-----------------------------------------------+
	// 環境マップオブジェクトの描画
	//-----------------------------------------------+
	// シェーダを有効化・uniformに各行列をセット
	m_renderer->m_environmentMapShader->SetActive();
	m_renderer->m_environmentMapShader->SetMatrixUniform("u_view", view);
	m_renderer->m_environmentMapShader->SetMatrixUniform("u_projection", proj);
	m_renderer->m_environmentMapShader->SetVectorUniform("u_viewPos", view.GetTranslation());
	m_renderer->m_environmentMapShader->SetInt("u_skybox", 0);
	for (auto env : m_renderer->m_envMeshComponents)
	{
		env->DrawEnvironmentMap(m_renderer->m_environmentMapShader);
	}



	if (m_renderer->m_switchShader == 0)
	{
		// フレームバッファ書き込み処理
		m_renderer->m_frameBuffer->WriteFrameBuffer();

		// マルチレンダリングで高輝度バッファを抽出し、ガウスぼかしを行う
		m_renderer->m_bloom->WriteBuffer(m_renderer->m_meshComponents, m_renderer->m_skeletalMeshComponents, m_renderer->m_activeSkyBox, m_renderer->m_envMeshComponents);
		m_renderer->m_bloom->WriteBuffer(m_renderer->m_particleManager);
		m_renderer->m_bloom->DrawDownSampling(m_renderer->GetBloom()->GetBrightBuffer());
		m_renderer->m_bloom->DrawGaussBlur();
		m_renderer->m_bloom->DrawBlendBloom(m_renderer->GetBloom()->GetColorBuffer());

	}


	//---------------------------------------------------------------+
	// スカイボックスの描画
	//---------------------------------------------------------------+
	// キューブマップシェーダをアクティブ化・キューブVAOをバインド
	//m_activeSkyBox->Draw(m_skyboxShader);

	//----------------------------------------------------------------+
	// パーティクル描画
	//----------------------------------------------------------------+
	//glEnable(GL_DEPTH_TEST);
	//m_particleManager->Draw();
	// ワールド空間上のスプライト描画
	m_renderer->m_worldSpaceSpriteShader->SetActive();
	m_renderer->m_worldSpaceSpriteShader->SetMatrixUniform("u_view", view);
	m_renderer->m_worldSpaceSpriteShader->SetMatrixUniform("u_projection", proj);
	for (auto spr : m_renderer->m_worldSprites)
	{
		spr->Draw(m_renderer->m_worldSpaceSpriteShader);
	}
	// フレームバッファ描画
	m_renderer->m_frameBuffer->DrawFrameBuffer();

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
