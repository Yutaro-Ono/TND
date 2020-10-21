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
#include "ChainUI.h"
#include "TutorialUI.h"
#include "LoadScreen.h"
#include "ResultScene.h"
#include "Math.h"
#include "PlayerCar.h"
#include "PlayerManager.h"
#include "GameWorld.h"

#include <Windows.h>
#include <iostream>

// コンストラクタ
GameScene::GameScene(int in_stageNum)
	:m_time(nullptr)
	,m_stageNum(in_stageNum)
	,m_state(STATE_FREE)
	,m_score(nullptr)
	,m_prevScore(0)
	,m_nowScore(0)
	,m_bestSpeed(0.0f)
	,m_cameraMove(-10.0f)
{

	// プロジェクション行列セット
	Matrix4 projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(75.0f),
		static_cast<float>(GAME_INSTANCE.GetRenderer()->GetScreenWidth()),
		static_cast<float>(GAME_INSTANCE.GetRenderer()->GetScreenHeight()),
		1.0f, 100000.0f);
	RENDERER->SetProjectionMatrix(projection);
	
	// ライティング
	GAME_INSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.0f, 0.04f, 0.15f));
	DirectionalLight& dir = GAME_INSTANCE.GetRenderer()->GetDirectionalLight();
	dir.m_position = Vector3(0.0f, 0.0f, 1000.0f);
	dir.m_direction = Vector3(0.0f, 0.0f, -1.0f);
	dir.m_direction.Normalize();
	dir.m_diffuseColor = Vector3(0.07f, 0.05f, 0.3f);
	dir.m_specColor = Vector3(0.01f, 0.0f, 0.1f);
}


// デストラクタ
GameScene::~GameScene()
{
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
		RENDERER->Draw();
	}

	//---------------------------------------------------------------------------------------------+
    // アクター
    //---------------------------------------------------------------------------------------------+
	// ワールド生成
	m_world = new GameWorld();

	// ロード画面処理
	Loading();

	// ウェイト
	Wait(300);
	// ロード画面処理
	Loading();


	// ウェイト
	Wait(300);
	// ロード画面処理
	Loading();

	// ロード画面処理
	Loading();
	// ウェイト
	Wait(300);
	// ロード画面処理
	Loading();
	// ウェイト
	Wait(300);
	// ロード画面処理
	Loading();
	// ウェイト
	Wait(300);
	// ロード画面処理
	Loading();

	// SkyBox
	SkyBox* sky = new SkyBox();

	// ウェイト
	Wait(300);
	// ロード画面処理
	Loading();


	// ウェイト
	Wait(300);
	// ロード画面処理
	Loading();

	//---------------------------------------------------------------------------------------------+
    // 音楽
    //---------------------------------------------------------------------------------------------+
	m_sound["BGM"] = "Data/Music/BGM/FC/GameScene/disent.wav";

	AUDIO->GetMusic(m_sound["BGM"]);

	// ウェイト
	Wait(300);
	// ロード画面処理
	Loading();

	// ウェイト
	Wait(300);
	// ロード画面処理
	Loading();

	// ウェイト
	Wait(300);

	// ロード画面の無効化
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();

	//---------------------------------------------------------------------------------------------+
	// インターフェース
	//---------------------------------------------------------------------------------------------+
	// HUD(プレイヤー)
	//HUD* hud = new HUD(m_player);
	//SpeedMeterUI* speedUI = new SpeedMeterUI(m_player);

	// タイムルール
	RuleTime* ruleTime = new RuleTime(this);
	m_time = ruleTime;
	CountDownUI* countUI = new CountDownUI(ruleTime);

}

// 更新処理
SceneBase * GameScene::Update()
{

	m_world->Update(GAME_INSTANCE.GetDeltaTime());

	// シーンの状態により処理を分岐
	switch (m_state)
	{

	case STATE_FREE:   // フリーモード

		// カウントダウン開始へ
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_SPACE) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A))
		{


			// スコア
			//RuleScore* ruleScore = new RuleScore(m_player);
			//m_score = ruleScore;
			//ScoreUI* scoreUI = new ScoreUI(ruleScore);
			// チェインUI
			//ChainUI* chainUI = new ChainUI(ruleScore);
			// 制限時間のセット
			m_time->SetStartTime();

			// カメラ位置初期化
			//m_camera->SetBestCameraDist();


			// UI順序の反転
			//GAME_INSTANCE.SwapPauseUI();

			// BGM再生
			//AUDIO->PlayMusic(m_sound["BGM"]);

			// カウントダウンへ
			m_state = STATE_START;
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
			//m_state = STATE_FINISH;
		}

		break;


	case STATE_FINISH:

		// 制限時間クラスからゲーム終了の合図が出たら終了
		if (m_time->GetGameEnd() == true)
		{
			//m_player->AllStopSound();

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

			// 次のシーンを返す
			return new ResultScene(m_score->GetScore(), m_bestSpeed);
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
