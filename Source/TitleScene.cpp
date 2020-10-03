//-----------------------------------------------------------------------+
// タイトルシーンクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "TitleScene.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include "TitleScreen.h"
#include "Font.h"
#include "GameScene.h"
#include "PlayerCar.h"
#include "PlayerCamera.h"
#include "Skydome.h"
#include "LoadScreen.h"

const int TitleScene::STAGE_ALL_NUM = 1;


// コンストラクタ
TitleScene::TitleScene()
	:m_state(PRESS_ANY_KEY)
	,m_selectedStage(0)
{
	//ライティング
	GAME_INSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.5f, 0.56f, 0.6f));
	DirectionalLight& dir = GAME_INSTANCE.GetRenderer()->GetDirectionalLight();
	dir.m_position = Vector3(0.0f, 0.0f, 1000.0f);
	dir.m_direction = Vector3(0.0f, 0.0f, 1.0f);
	dir.m_direction.Normalize();
	dir.m_diffuseColor = Vector3(0.0f, 1.0f, 0.6f);
	dir.m_specColor = Vector3(0.0f, 1.0f, 0.0f);
}


// デストラクタ
TitleScene::~TitleScene()
{
	// プレイヤーを削除
	m_player->SetState(Actor::STATE_DEAD);
	// スカイドームを削除
	//m_skydome->SetState(Actor::STATE_DEAD);

}


// 初期化処理
void TitleScene::Initialize()
{
	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}



	// タイトル用UI
	TitleScreen* hud = new TitleScreen(this);

	//m_skydome = new Skydome();
	//m_skydome->SetMesh(RENDERER->GetMesh("Data/Meshes/FC/Skydome/Skydome_Sunny.gpmesh"));
	//m_skydome->SetPosition(Vector3::Zero);
	//m_skydome->SetScale(1.0f);

	// プレイヤー
	m_player = new PlayerCar();
	m_player->SetPosition(Vector3::Zero);
	m_player->SetScale(0.5f);
	m_player->SetState(Actor::STATE_PAUSED);

	// カメラ
	Camera* camera = new Camera(m_player);
	camera->Initialize(Vector3(0.0f, -250.0f, 0.0f), m_player->GetPosition(), Vector3::Zero);



	// 音楽
	m_sound["BGM"] = "Data/Music/BGM/FC/TitleScene/neighofwar.wav";
	m_sound["Enter"] = "Data/Music/SE/FC/System/Enter/switch01.wav";

	// SE
	AUDIO->GetSound(m_sound["Enter"]);                                       // 決定音

	// BGM
	//AUDIO->GetMusic(m_sound["BGM"]);
	//AUDIO->PlayMusic(m_sound["BGM"]);


	// ロード画面の無効化
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
}


// 更新処理
SceneBase * TitleScene::Update()
{
	// タイトルメニュー
	switch (m_state)
	{



	//----------------------------------------------------------------------+
	// "PRESS ANY KEY"
	//----------------------------------------------------------------------+
	case PRESS_ANY_KEY:


		// いずれかのボタンで次へ
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A)|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_X)|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_Y)
			|| CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);
			m_state = GAME_START;
		}

		break;




	//----------------------------------------------------------------------+
	// "GAME START"
	//----------------------------------------------------------------------+
	case GAME_START:

		// TABかBボタンを押したらメニュー画面戻る
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = PRESS_ANY_KEY;
			break;
		}


		// 下キーかDPAD下で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = GAME_QUIT;
		}


		// ステージセレクトへ( KEYBOARD : SPACE or ENTER | XINPUT : A )
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			// ステージセレクトへ
			m_state = STAGE_SELECT;
		}

		break;




	//-----------------------------------------------------------------------------------+
	// "STAGE SELECT"
	//-----------------------------------------------------------------------------------+
	case STAGE_SELECT:

		//---------------------------------------------------------------------+
		// 「戻る」処理
		//---------------------------------------------------------------------+
		// TABかBボタンを押したらメニュー画面戻る
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			// "GAME_START"へ
			m_state = GAME_START;


			break;
		}


		//-----------------------------------------------------------------------+
		// ステージ項目の移動時処理
		//-----------------------------------------------------------------------+
		// ↑キーかDPAD↑で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			// ステージ1でなければ選択中のステージをカウントダウン
			if (m_selectedStage != 0)
			{
				m_selectedStage--;
			}

			break;
		}

		// ↓キーかDPAD↓で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			// ステージ数の上限でなければ選択中のステージ番号をカウントアップ
			if (m_selectedStage < STAGE_ALL_NUM - 1)
			{
				m_selectedStage++;
			}

			break;
		}


		//---------------------------------------------------------------------------+
		// シーン遷移処理
		//---------------------------------------------------------------------------+
		// SPACEかENTER、Aボタンを押したら次のシーンへ
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);
			// 音楽をフェードアウト
			AUDIO->FadeOutMusic(1.0f);

			// プレイヤーのサウンドを停止
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
			return new GameScene(m_selectedStage);
		}

		break;




	//----------------------------------------------------------------------+
	// "QUIT TO DESKTOP"
	//----------------------------------------------------------------------+
	case GAME_QUIT:


		// SPACEかENTER、Aボタンを押したらシャットダウン
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			GAME_INSTANCE.SetShutDown();
			break;
		}

		// TABかBボタンを押したらメニュー画面戻る
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_TAB) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_B))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = PRESS_ANY_KEY;
			break;
		}


		// 上キーかDPAD上で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// 決定音
			AUDIO->PlaySoundTND(m_sound["Enter"]);

			m_state = GAME_START;
		}

		break;




	default:

		break;


	}

	
	return this;

}


// 描画処理
void TitleScene::Draw()
{
	RENDERER->Draw();
}
