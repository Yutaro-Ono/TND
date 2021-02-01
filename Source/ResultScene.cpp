//-----------------------------------------------------------------------+
// リザルトシーンクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "ResultScene.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Input.h"
#include "InputController.h"
#include "TitleScene.h"
#include "Actor.h"
#include "LevelObject.h"
#include "Camera.h"
#include "ResultScreen.h"
#include "LoadScreen.h"
#include "RecordScore.h"
#include "Environment.h"


// コンストラクタ
ResultScene::ResultScene(int in_score, float in_bestSpeed)
	:m_state(STATE_SCORE_DISPLAY)
	,m_score(in_score)
	,m_environment(nullptr)
	,m_bestSpeed(in_bestSpeed)
{

}

// デストラクタ
ResultScene::~ResultScene()
{
	GAME_INSTANCE.DeadAllActor();
	// 全てのUIをCloseに設定
	for (auto iter : GAME_INSTANCE.GetUIStack())
	{
		iter->Close();
	}
}


// 初期化処理
void ResultScene::Initialize()
{
	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}

	// 音楽
	//m_sound["BGM"] = "Data/Music/BGM/FC/ResultScene/townofdeath.wav";
	m_sound["Enter"] = "Data/Music/SE/TND/System/Enter/decide13.wav";
	//AUDIO->GetMusic(m_sound["BGM"]);
	AUDIO->GetSound(m_sound["Enter"]);

	m_environment = new Environment(Environment::MORNING);

	// ロード処理
	for (int i = 0; i < 64; i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// ロード画面の無効化
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();

	// レコード
	m_record = new RecordScore(m_score);

	// UI
	m_resultUI = new ResultScreen(this, m_score, m_bestSpeed);
	m_resultUI->SetRank(m_record->GetRank());
	// UIへのスコア情報の受け渡し
	for (int i = 0; i < 10; i++)
	{
		m_resultUI->SetScore(i, m_record->GetRankingScore(i));
	}
}


// シーンの更新処理
SceneBase * ResultScene::Update()
{

	switch (m_state)
	{

	case STATE_SCORE_DISPLAY:

		if (m_resultUI->GetDispScore() == m_score)
		{
			m_state = STATE_ONE_MORE;
		}


		break;

	case STATE_ONE_MORE:

		// BGM再生
		//if (AUDIO->IsPlayingMusic() == false)
		//{
		//	AUDIO->PlayMusic(m_sound["BGM"]);
		//}

		// 矢印キー右かコントローラーのDPAD右で次の項目へ
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RIGHT) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		{
			// 決定音再生
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = STATE_GAME_QUIT;
		}

		// SPACEかENTER、Aボタンを押したらリプレイ (次のシーンを返す)
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// 決定音再生
			AUDIO->PlaySoundTND(m_sound["Enter"]);

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

			// レコードを削除
			delete m_record;

			// タイトルシーンのポインタを返す
			return new TitleScene;

			break;
		}

		break;

	case STATE_GAME_QUIT:

		// 矢印キー左かコントローラーのDPAD左で前の項目へ
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LEFT) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
		{
			// 決定音再生
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = STATE_ONE_MORE;
		}

		// SPACEかENTER、Aボタンを押したらシャットダウン
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// 決定音再生
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			GAME_INSTANCE.SetShutDown();
			break;
		}

		break;
	}

	return this;
}


// 描画処理
void ResultScene::Draw()
{
	RENDERER->Draw();
}
