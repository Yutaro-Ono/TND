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
#include "ResultCar.h"
#include "BridgeObject.h"
#include "RenderBloom.h"

// コンストラクタ
ResultScene::ResultScene(int in_score, float in_bestSpeed)
	:m_state(STATE_WHITE_IN)
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
	// 環境生成
	m_environment = new Environment(Environment::GAME_TIME::MORNING, Vector3(0.0f, -65.0f, 0.0f));

	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}

	// プレイヤー
	m_car = new ResultCar();
	m_car->SetPosition(Vector3(6500.0f, -65.0f, 0.0f));
	m_car->SetScale(0.4f);


	for (int i = 0; i < 8; i++)
	{
		m_bridge[i] = new BridgeObject(1, Vector3(i * 6500.0f, -2000.0f, 0.0f), false);

		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// 音楽
	m_sound["BGM"] = "Data/Music/BGM/TND/Result/cyrf_lu_metropolis.wav";
	m_sound["Enter"] = "Data/Music/SE/TND/System/Enter/decide13.wav";
	m_sound["Select"] = "Data/Music/SE/TND/System/Select/decide14.wav";
	AUDIO->GetMusic(m_sound["BGM"]);
	AUDIO->GetSound(m_sound["Enter"]);
	AUDIO->GetSound(m_sound["Select"]);

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

	m_environment->SetDirectionalLightPos(m_car->GetPosition());

	switch (m_state)
	{


	//----------------------------------------------------------------------+
    // "WHITE IN"
    //----------------------------------------------------------------------+
	case STATE_WHITE_IN:


		if (RENDERER->GetBloom()->WhiteIn(70.3f, GAME_INSTANCE.GetDeltaTime()))
		{
			m_state = STATE_SCORE_DISPLAY;
		}

		break;


	//----------------------------------------------------------------------+
	// "WHITE OUT"
	//----------------------------------------------------------------------+
	case STATE_WHITE_OUT:

		if (RENDERER->GetBloom()->WhiteOut(70.3f, GAME_INSTANCE.GetDeltaTime()))
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

			// レコードを削除
			delete m_record;

			// 次のシーンを返す
			return new TitleScene;
		}

		break;


	case STATE_SCORE_DISPLAY:

		// BGM再生
		if (AUDIO->IsPlayingMusic() == false)
		{
			AUDIO->PlayMusic(m_sound["BGM"]);
		}

		if (m_resultUI->GetDispScore() == m_score)
		{
			m_state = STATE_ONE_MORE;
		}


		break;

	case STATE_ONE_MORE:



		// 矢印キー右かコントローラーのDPAD右で次の項目へ
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RIGHT) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		{
			// 決定音再生
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = STATE_GAME_QUIT;
		}

		// SPACEかENTER、Aボタンを押したらリプレイ (次のシーンを返す)
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// 決定音再生
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			AUDIO->FadeOutMusic(3000.0f);

			m_state = STATE_WHITE_OUT;

			break;
		}

		break;

	case STATE_GAME_QUIT:

		// 矢印キー左かコントローラーのDPAD左で前の項目へ
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_LEFT) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
		{
			// 決定音再生
			AUDIO->PlaySoundTND(m_sound["Select"]);

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
