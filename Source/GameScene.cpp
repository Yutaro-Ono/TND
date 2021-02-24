//---------------------------------------------------------------------------+
// ゲームシーンクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//---------------------------------------------------------------------------+
#include "GameScene.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "AudioManager.h"
#include "Renderer.h"
#include "ParticleManager.h"
#include "LevelManager.h"
#include "SkyBox.h"
#include "RuleScore.h"
#include "RuleTime.h"
#include "HUD.h"
#include "CountDownUI.h"
#include "SpeedMeterUI.h"
#include "ScoreUI.h"
#include "TutorialUI.h"
#include "LoadScreen.h"
#include "ResultScene.h"
#include "Math.h"
#include "PlayerCar.h"
#include "PlayerManager.h"
#include "GameWorld.h"
#include "TitleScene.h"
#include "Canvas.h"
#include "RenderBloom.h"
#include <Windows.h>
#include <iostream>

// コンストラクタ
GameScene::GameScene(int in_stageNum)
	:m_time(nullptr)
	,m_stageNum(in_stageNum)
	,m_state(STATE_FADE_IN)
	,m_score(nullptr)
	,m_prevScore(0)
	,m_nowScore(0)
	,m_bestSpeed(0.0f)
	,m_cameraMove(-10.0f)
{


}


// デストラクタ
GameScene::~GameScene()
{
	delete m_world;
	// 全てのアクターを削除
	GAME_INSTANCE.DeadAllActor();
	// 全てのUIをCloseに設定
	for (auto iter : GAME_INSTANCE.GetUIStack())
	{
		iter->Close();
	}
	// パーティクルを全て削除
	RENDERER->GetParticleManager()->AllDeadParticle();
	// 音楽を停止
	AUDIO->StopMusic();
}


// 初期化処理
// 非マルチスレッド下でロード処理を導入しているため視認性が悪い
void GameScene::Initialize()
{
	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
		GAME_INSTANCE.GetLoadScreen()->SetIsGame();
	}

	//---------------------------------------------------------------------------------------------+
    // アクター
    //---------------------------------------------------------------------------------------------+
	// ワールド生成
	m_world = new GameWorld();

	//---------------------------------------------------------------------------------------------+
    // 音楽
    //---------------------------------------------------------------------------------------------+
	m_sound["BGM"] = "Data/Music/BGM/TND/Game/cyrf_cloudbase (mp3cut.net).wav";
	AUDIO->GetMusic(m_sound["BGM"]);

	// ロード画面の無効化
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();

	// タイムルール
	RuleTime* ruleTime = new RuleTime(this);
	m_time = ruleTime;
	// 制限時間のセット
	m_time->SetStartTime();

	CountDownUI* countUI = new CountDownUI(ruleTime);

	// BGM再生開始
	AUDIO->PlayMusic(m_sound["BGM"]);
}

// 更新処理
SceneBase * GameScene::Update()
{

	m_world->Update(GAME_INSTANCE.GetDeltaTime());

	// シーンの状態により処理を分岐
	switch (m_state)
	{

	//----------------------------------------------------------------------+
    // "FADE IN"
    //----------------------------------------------------------------------+
	case STATE_FADE_IN:


		if (RENDERER->GetBloom()->FadeIn(0.8f, GAME_INSTANCE.GetDeltaTime()))
		{
			m_state = STATE_FREE;
		}

		break;


	//----------------------------------------------------------------------+
	// "FADE OUT"
	//----------------------------------------------------------------------+
	case STATE_FADE_OUT:

		if (RENDERER->GetBloom()->WhiteOut(70.3f, GAME_INSTANCE.GetDeltaTime()))
		{
			// 次のシーンを返す
			return new ResultScene(m_world->GetCanvas()->GetScoreUI()->GetScore(), m_bestSpeed);
		}

		break;


	case STATE_FREE:   // フリーモード

		// カウントダウン開始へ
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_SPACE) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A))
		{
			// 制限時間のセット
			m_time->SetStartTime();

			// カウントダウンへ
			m_state = STATE_START;
		}

		// 制限時間のセット
		m_time->SetStartTime();

		// カウントダウンへ
		m_state = STATE_START;

		// デバッグ用タイトルシーン
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_P) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A))
		{
			//return new TitleScene();
		}



		break;

	case STATE_START:   // スタート時処理

		// プレイヤーをポーズする
		//m_player->SetState(m_player->STATE_PAUSED);

		// カメラをプレイヤーに寄せる
		if (m_time->GetNowCount() < 3)
		{
			//m_camera->SetBestCameraDist();
		}
		else
		{
			//m_cameraMove -= 3.0f;
			//m_camera->SetHorizonDist(m_cameraMove);
		}

		// カウント終了でアクティブに
		if (m_time->GetNowCount() < 0)
		{
			//m_player->SetState(m_player->STATE_ACTIVE);

			// 制限時間のセット
			m_time->SetLimitTime();

			m_state = STATE_IN_GAME;
		}

		break;


	case STATE_IN_GAME:

		// 現在のスコアを記録
		//m_nowScore = m_score->GetScore();
		//
		//// スコアが変動したらカウントアップ
		//if (m_nowScore != m_prevScore)
		//{
		//	m_time->AddCount(1.0f);
		//	m_prevScore = m_nowScore;
		//}

		// プレイヤーの速度を監視し、最高速度を更新したら記録を書き換え
		//if (m_bestSpeed < m_player->GetAccel())
		//{
		//	m_bestSpeed = m_player->GetAccel();
		//}

		// ステート変更
		if (m_time->GetCountState() == m_time->STATE_FINISH)
		{
			m_state = STATE_FINISH;
		}

		break;


	case STATE_FINISH:

		// 制限時間クラスからゲーム終了の合図が出たら終了
		if (m_time->GetGameEnd() == true)
		{
			//m_player->AllStopSound();

			// マップHUDのオフ


			m_state = STATE_FADE_OUT;
		}

		break;

	default:

		break;

	}

	// ゲームフィニッシュ時、プレイヤーをストップしカメラを離す
	if (m_time->GetCountState() == m_time->STATE_FINISH)
	{
		//m_player->SetState(m_player->STATE_PAUSED);

		//m_camera->SetHorizonDist(m_camera->GetPosition().x * 0.2f);
		//m_camera->SetVerticalDist(500.0f);
	}

	return this;
}

// 描画処理
void GameScene::Draw()
{
	// 描画
	RENDERER->Draw();
}

// ウェイト処理
void GameScene::Wait(int in_millisec)
{
	Sleep(in_millisec);
}

// ローディング処理
void GameScene::Loading()
{
	GAME_INSTANCE.GetLoadScreen()->AddGauge();
	RENDERER->Draw();
}
