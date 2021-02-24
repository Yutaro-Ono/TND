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
#include "LoadScreen.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Environment.h"
#include "WorldSpaceUI.h"
#include "ClientActor.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "TitleCar.h"
#include "BridgeObject.h"
#include "RenderBloom.h"
#include "TutorialScene.h"
const int TitleScene::STAGE_ALL_NUM = 1;

// コンストラクタ
TitleScene::TitleScene()
	:m_state(FADE_IN)
	,m_selectedStage(0)
	,m_car(nullptr)
	,m_client(nullptr)
	,m_environment(nullptr)
{
}

// デストラクタ
TitleScene::~TitleScene()
{
	GAME_INSTANCE.DeadAllActor();
	delete m_environment;
}

// 初期化処理
void TitleScene::Initialize()
{
	// 環境生成
	m_environment = new Environment(Environment::GAME_TIME::NIGHT, Vector3(0.0f, -65.0f, 0.0f));
	//DirectionalLight& light = RENDERER->GetDirectionalLight();
	//light.target = Vector3::Zero;
	//light.position = light.target + Vector3(2000.0f, 0.0f, 3000.0f);
	//light.direction = light.target - light.position;
	//light.direction.Normalize();
	//m_environment = new Environment(Environment::GAME_TIME::MORNING);
	// ロード画面処理
	{
		GAME_INSTANCE.GetLoadScreen()->EnableScreen();
	}

	// ロード処理
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// 音楽
	m_sound["BGM"] = "Data/Music/BGM/TND/Title/cyrf_crashed_dimension (mp3cut.net).wav";
	// ロード処理
	GAME_INSTANCE.GetLoadScreen()->AddGauge();
	m_sound["Enter"] = "Data/Music/SE/TND/System/Enter/decide13.wav";
	// ロード処理
	GAME_INSTANCE.GetLoadScreen()->AddGauge();
	m_sound["Select"] = "Data/Music/SE/TND/System/Select/decide14.wav";

	// SE
	AUDIO->GetSound(m_sound["Enter"]);                                       // 決定音
	AUDIO->GetSound(m_sound["Select"]);                                      // 選択音

	// ロード処理
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// BGM
	AUDIO->GetMusic(m_sound["BGM"]);

	// ロード処理
	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// プレイヤー
	//m_car = new PlayerCar();
	m_car = new TitleCar();
	m_car->SetPosition(Vector3(6500.0f, -65.0f, 0.0f));
	m_car->SetScale(0.4f);
	//m_car->SetState(Actor::STATE_PAUSED);

	GAME_INSTANCE.GetLoadScreen()->AddGauge();

	// 橋オブジェクトの生成
	for (int i = 0; i < 8; i++)
	{
		m_bridge[i] = new BridgeObject(1, Vector3(i * 6515.0f, -2000.0f, 0.0f));

		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	
	//m_client = new ClientActor(Vector3::Zero, 5);
	//m_client->SetPosition(Vector3(0.0f, -55.0f, 0.0f));
	//m_client->SetScale(0.4f);



	//m_pointLight = new PointLight();
	//m_pointLight->SetPosition(Vector3(0.0f, 0.0f, 40.0f));

	//m_spotLight = new SpotLight(Vector3::Zero, SpotLight::VL_BIG);

	for (int i = 0; i < 61; i++)
	{
		GAME_INSTANCE.GetLoadScreen()->AddGauge();
	}

	// ロード画面の無効化
	GAME_INSTANCE.GetLoadScreen()->DisableScreen();
	// タイトル用UI
	TitleScreen* hud = new TitleScreen(this);

	// BGM再生開始
	AUDIO->PlayMusic(m_sound["BGM"]);
}

// 更新処理
SceneBase * TitleScene::Update()
{

	// 環境光更新
	m_environment->SetDirectionalLightPos(m_car->GetPosition());


	// タイトルメニュー
	switch (m_state)
	{

	//----------------------------------------------------------------------+
    // "FADE IN"
    //----------------------------------------------------------------------+
	case FADE_IN:


		if (RENDERER->GetBloom()->FadeIn(0.5f, GAME_INSTANCE.GetDeltaTime()))
		{
			m_state = PRESS_ANY_KEY;
		}

		break;


	//----------------------------------------------------------------------+
    // "FADE OUT"
    //----------------------------------------------------------------------+
	case FADE_OUT:

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

			// 次のシーンを返す
			return new GameScene(m_selectedStage);
			//return new TutorialScene();
		}
		
		break;


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
			AUDIO->PlaySoundTND(m_sound["Select"]);
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
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = PRESS_ANY_KEY;
			break;
		}

		// 下キーかDPAD下で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
		{
			// 選択音
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = GAME_QUIT;
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
			AUDIO->FadeOutMusic(3000.0f);
			
			

			// フェードアウト処理に移行
			m_state = FADE_OUT;
		}

		//// ステージセレクトへ( KEYBOARD : SPACE or ENTER | XINPUT : A )
		//if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_SPACE) || INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_RETURN) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_A))
		//{
		//	// 決定音
		//	AUDIO->PlaySoundTND(m_sound["Enter"]);

		//	// ステージセレクトへ
		//	m_state = STAGE_SELECT;
		//}

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
			AUDIO->PlaySoundTND(m_sound["Select"]);

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
			// 選択音
			AUDIO->PlaySoundTND(m_sound["Select"]);

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
			// 選択音
			AUDIO->PlaySoundTND(m_sound["Select"]);

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

			// フェードアウト処理に移行
			m_state = FADE_OUT;
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
			AUDIO->PlaySoundTND(m_sound["Select"]);

			m_state = PRESS_ANY_KEY;
			break;
		}


		// 上キーかDPAD上で選択
		if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_DPAD_UP))
		{
			// 選択音
			AUDIO->PlaySoundTND(m_sound["Select"]);

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
