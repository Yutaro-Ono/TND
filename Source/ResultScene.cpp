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
#include "Skydome.h"
#include "LevelObject.h"
#include "Camera.h"
#include "ResultScreen.h"
#include "LoadScreen.h"
#include "RecordScore.h"


// コンストラクタ
ResultScene::ResultScene(int in_score, float in_bestSpeed)
	:m_state(STATE_SCORE_DISPLAY)
	,m_score(in_score)
	,m_bestSpeed(in_bestSpeed)
{
	//ライティング
	GAME_INSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.5f, 0.56f, 0.6f));
	DirectionalLight& dir = GAME_INSTANCE.GetRenderer()->GetDirectionalLight();
	dir.m_direction = Vector3(0.0f, 0.0f, 1.0f);
	dir.m_direction.Normalize();
	dir.m_diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	dir.m_specColor = Vector3(0.8f, 0.8f, 0.8f);
}


// デストラクタ
ResultScene::~ResultScene()
{

}


// 初期化処理
void ResultScene::Initialize()
{
	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
		RENDERER->Draw();
	}

	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}



	// スカイドームの生成と設定
	m_skydome = new Skydome();
	m_skydome->SetMesh(RENDERER->GetMesh("Data/Meshes/FC/Skydome/Skydome_Sunny.gpmesh"));
	m_skydome->SetPosition(Vector3(0.0f, 0.0f, -20.0f));
	m_skydome->SetScale(0.45f);

	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}


	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}



	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}
	

	//------------------------------------------------------------//
	// オブジェクトの生成
	//-----------------------------------------------------------//
	// 基地
	LevelObject* m_platform = new LevelObject(RENDERER->GetMesh("Data/Meshes/FC/Objects/Result_Building/SM_Platform_A.gpmesh"));
	m_platform->SetPosition(Vector3(0.0f, 0.0f, -220.0f));
	m_platform->SetScale(0.6f);

	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}

	// 柱
	LevelObject* m_pillar = new LevelObject(RENDERER->GetMesh("Data/Meshes/FC/Objects/Result_Building/SM_MapRoom_Pillars.gpmesh"));
	m_pillar->SetPosition(Vector3(0.0f, 0.0f, -220.0f));
	m_pillar->SetScale(0.6f);

	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}


	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}



	// カメラの生成と初期化
	//Camera* camera = new Camera();

	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}


	// 音楽
	m_sound["BGM"] = "Data/Music/BGM/FC/ResultScene/townofdeath.wav";
	m_sound["Enter"] = "Data/Music/SE/FC/System/Enter/switch01.wav";
	AUDIO->GetMusic(m_sound["BGM"]);
	AUDIO->GetSound(m_sound["Enter"]);


	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
	}





	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
		RENDERER->Draw();
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
		if (AUDIO->IsPlayingMusic() == false)
		{
			AUDIO->PlayMusic(m_sound["BGM"]);
		}


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
