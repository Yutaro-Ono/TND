#include "TutorialScene.h"
#include "GameMain.h"
#include "Renderer.h"
#include "RenderBloom.h"
#include "PlayerManager.h"
#include "BridgeObject.h"
#include "LoadScreen.h"
#include "GameScene.h"
#include "Environment.h"
#include "ParticleManager.h"

TutorialScene::TutorialScene()
	:m_state(STATE_FADE_IN)
	,m_player(nullptr)
{

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{

	// 環境生成
	m_environment = new Environment(Environment::GAME_TIME::NIGHT, Vector3(0.0f, 0.0f, 0.0f));

	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}

	// プレイヤー(人＆車)生成
	m_player = new PlayerManager();
	m_player->SetPosition(Vector3(0.0f, -1000.0f, 10.0f));
	m_player->SetScale(0.3f);
	// ロード処理
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// 橋オブジェクトの生成
	for (int i = 0; i < 12; i++)
	{
		m_bridge[i] = new BridgeObject(1, Vector3(i * 6515.0f, 0.0f, 0.0f));

		// ロード処理
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// ロード画面の無効化
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns> 通常は自身のポインタを、シーン遷移の条件を達成した場合は次シーンへのポインタを返す </returns>
SceneBase* TutorialScene::Update()
{

	// 一定距離進んだら次のシーンへ
	float toNextX = 30000.0f;                  // この座標を超えたら次シーンへ
	if (m_player->GetPosition().x > toNextX)
	{
		m_state = STATE_FADE_OUT;
	}


	switch (m_state)
	{
	case STATE_FADE_IN:

		if (RENDERER->GetBloom()->FadeIn(0.5f, GAME_INSTANCE.GetDeltaTime()))
		{
			m_state = STATE_0_MOVING;
		}

		break;

	case STATE_FADE_OUT:

		if (RENDERER->GetBloom()->FadeOut(0.7f, GAME_INSTANCE.GetDeltaTime()))
		{

			// 全てのUIをCloseに設定
			for (auto iter : GAME_INSTANCE.GetUIStack())
			{
				iter->Close();
			}

			// 全てのアクターを削除
			for (auto actor : GAME_INSTANCE.GetActorStack())
			{
				actor->SetState(Actor::STATE_DEAD);
			}

			// パーティクルを全て削除
			RENDERER->GetParticleManager()->AllDeadParticle();

			// 次のシーンを返す
			return new GameScene(0);
		}

		break;

	case STATE_0_MOVING:

		break;

	case STATE_1_RIDING:

		break;

	case STATE_2_DRIVING:


		break;





	}


	return this;
}

void TutorialScene::Draw()
{
	RENDERER->Draw();
}
