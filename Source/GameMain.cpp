//-----------------------------------------------------------------------+
// ゲームループ.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+

// インクルードファイル
#include "GameMain.h"
#include "SceneBase.h"
#include "Camera.h"
#include "Actor.h"
#include "Camera.h"
#include "CameraComponent.h"
#include "PhysicsWorld.h"
#include "Collision.h"
#include "DebugBox.h"
#include "ParticleManager.h"
#include "AudioManager.h"
#include "UIScreen.h"
#include "Font.h"
#include "PauseScreen.h"
#include "LoadScreen.h"
#include "glad/glad.h"
#include <algorithm>
#include <utility>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>

// 標準フォント
const std::string GameMain::FONT_FILE_PATH = "Data/Fonts/SackersGothicLightAT.ttf";

// コンストラクタ
GameMain::GameMain()
	:m_state(PLAYING)
	,m_renderer(nullptr)
	,m_debugWindow(nullptr)
	,m_debugRenderer(nullptr)
	,m_audio(nullptr)
	,m_config(nullptr)
	,m_isRunning(true)
	,m_isPauseMode(false)
	,m_activeCamera(nullptr)
	,m_nowScene(nullptr)
	,m_physicsWorld(nullptr)
	,m_pause(nullptr)
	,m_load(nullptr)
{
	// 処理なし
}

// デストラクタ
GameMain::~GameMain()
{
	// 処理なし
}

// 各種初期化処理(SDL, Renderer)
// in_full -> フルスクリーンかどうか
bool GameMain::Initialize()
{
	//--------------------------------------------------------------------+
	// SDL
	//--------------------------------------------------------------------+
	// SDLの初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_Log("SDL Initialize Failed : %s\n", SDL_GetError());
		return false;
	}

	//--------------------------------------------------------------------+
    // コンフィグ
    //--------------------------------------------------------------------+
    // コンフィグの生成・ロード
	m_config = new GameConfig();
	m_config->LoadConfig();

	//--------------------------------------------------------------------+
	// レンダラー
	//--------------------------------------------------------------------+
	// レンダラーの生成
	m_renderer = new Renderer();
	// レンダラーの初期化
	if (!m_renderer->Initialize(GetConfig()->GetScreenWidth(), GetConfig()->GetScreenHeight(), GetConfig()->GetFullScreen()))
	{
		SDL_Log("Renderer Initialize Failed : %s\n", SDL_GetError());
		delete m_renderer;
		return false;
	}

	//--------------------------------------------------------------------+
	// フォント(TTF)レンダリングシステム初期化
	//--------------------------------------------------------------------+
	if (TTF_Init() != 0)
	{
		SDL_Log("SDL_ttf Initialize Failed : %s\n", SDL_GetError());
		return false;
	}

	//--------------------------------------------------------------------+
	// サウンドシステム初期化
	//--------------------------------------------------------------------+
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		delete m_renderer;
		return false;
	}
	printf("SDLMixer初期化完了\n");
	// SDLMixer API初期化　44100:音源の周波数, 44.1kHz 2:ステレオ 4096:内部バッファサイズ
	// 効果音はMP3使用不可
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		//delete m_renderer;
		//Mix_CloseAudio();
		//Mix_Quit();
		//return false;
	}
	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}
	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI初期化完了\n");

	m_audio = new AudioManager();

	// 当たり判定システム生成
	m_physicsWorld = new PhysicsWorld;

	// デバッグ用ボックス
	m_debugBox = new DebugBox;

	// 入力システム(コントローラー)初期化
	if (!CONTROLLER_INSTANCE.Initialize())
	{
		return false;
	}

	// 入力システム(ステアリング)初期化
	if (!STEERING_CONTROLLER_INSTANCE.Initialize())
	{
		return false;
	}

	// フレーム初期化
	m_frame = 0;

	// SDLが初期化されてから経過した時間(ミリ秒単位)
	m_ticksCount = SDL_GetTicks();

	// フォント生成
	Font* font = new Font();
	font->Load(FONT_FILE_PATH);
	m_fonts.emplace(FONT_FILE_PATH, font);

	// ポーズ画面生成
	m_pause = new PauseScreen();


	// ロード画面生成
	m_load = new LoadScreen();
	m_load->Initialize();


	return true;
}

// 各種解放処理
void GameMain::Delete()
{

	// アクターの削除　（アクターを通じてコンポーネントも削除される）
	while (!m_actors.empty())
	{
		delete m_actors.back();
	}
	while (!m_pendingActors.empty())
	{
		delete m_pendingActors.back();
	}
	if (m_renderer)
	{
		m_renderer->Delete();
	}

	// UI削除
	while (!m_uiStack.empty())
	{
		delete m_uiStack.back();
		m_uiStack.pop_back();
	}

	// オーディオの削除
	m_audio->ShutDown();
	delete m_audio;
	Mix_CloseAudio();
	Mix_Quit();

	delete m_physicsWorld;
	delete m_debugBox;

	// 入力関連の削除
	CONTROLLER_INSTANCE.Delete();
	STEERING_CONTROLLER_INSTANCE.Delete();

	// Imguiの削除
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// SDLウィンドウの破棄(デバッグ用)
	SDL_DestroyWindow(m_debugWindow);
	// SDLレンダラーの破棄(デバッグ用)
	SDL_DestroyRenderer(m_debugRenderer);

	SDL_Quit();


}

// メインループ
void GameMain::RunLoop()
{
	// 開始シーンが定義されていなかったら終了
	if (!m_nowScene)
	{
		printf("開始シーンが設定されていません");
		return;
	}

	// ゲームループ
	// m_isRunningがfalseならループを抜け終了
	while (m_isRunning)
	{
		Input();
		// LoopHelper();

		if (UpdateGame())
		{
			continue;
		}

		Draw();
		// DebugRenderer();
	}
}

// ゲームの更新処理
int GameMain::UpdateGame()
{
	// 16ミリ秒(= 60フレーム/秒)になるように、前のフレームから16ミリ秒以上経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16))
		;

	// フレーム更新
	m_frame++;

	// 前のフレームから現在時刻までの経過時間算出(秒単位）
	m_deltaTime = (SDL_GetTicks() - m_ticksCount + 16) / 1000.0f;

	//このフレームの開始時間保存（次のフレームまでの経過時間をはかるため）
	m_ticksCount = SDL_GetTicks();

	// フレーム時間があまりにも経過している場合は0.05 → 20fps固定
	if (m_deltaTime > 0.05f)
	{
		m_deltaTime = 0.05f;
	}

	if (!m_nowScene)
	{
		return 0;
	}

	// 毎フレーム、シーン情報を保存
	SceneBase* tmpScene;

	// アクターの処理
	UpdateActor();

	// カメラ更新
	if (m_activeCamera != nullptr)
	{
		// カメラ行列計算
		m_activeCamera->Update(m_deltaTime);
	}

	// 当たり判定
	m_physicsWorld->Collision();

	// パーティクル更新
	m_renderer->GetParticleManager()->Update(m_deltaTime);

	//シーンを実行
	tmpScene = m_nowScene->Update();

	// UIがCLOSEなら消す
	auto iter = m_uiStack.begin();
	while (iter != m_uiStack.end())
	{
		if ((*iter)->GetState() == UIScreen::CLOSE)
		{
			delete *iter;
			iter = m_uiStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	// UIの更新(CLOSEなら消す)
	for (auto ui : m_uiStack)
	{
		if (ui->GetState() == UIScreen::ACTIVE)
		{
			ui->Update(m_deltaTime);
		}

	}

	// シーン更新の判定処理
	if (tmpScene != m_nowScene)
	{

		// 現在のシーンを削除
		delete m_nowScene;

		//新規シーンを今のシーンに差し替え
		m_nowScene = tmpScene;

		// 物理処理を新規シーン用に更新する
		delete m_physicsWorld;
		m_physicsWorld = new PhysicsWorld();

		// ロード画面生成
		m_load = new LoadScreen();
		m_load->Initialize();

		// シーンの初期化
		m_nowScene->Initialize();

		// ポーズ画面生成
		m_pause = new PauseScreen();

		return 1;
	}


	return 0;

}

void GameMain::UpdateActor()
{
	// 全アクター更新
	for (auto actor : m_actors)
	{
		actor->Update(m_deltaTime);
	}

	// ペンディング中アクターをアクティブに移動
	for (auto pending : m_pendingActors)
	{
		pending->ComputeWorldTransform();
		m_actors.emplace_back(pending);
	}
	m_pendingActors.clear();

	// 全ての死亡しているアクターを一時保管
	std::vector<Actor*> deadActors;
	for (auto actor : m_actors)
	{
		if (actor->GetState() == Actor::STATE_DEAD)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死亡しているアクターをDelete
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();

}


// 終了処理
void GameMain::CloseGame()
{
	Delete();
}

// 最初のシーンをセットし初期化
void GameMain::SetFirstScene(SceneBase * in_scene)
{
	m_nowScene = in_scene;
	m_nowScene->Initialize();
}


// データのロード
void GameMain::Load()
{
}

// データのアンロード
void GameMain::Unload()
{
}

void GameMain::Input()
{
	// 終了イベントのキャッチ
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			m_isRunning = false;
			break;

		default:
			CONTROLLER_INSTANCE.ReceiveEvent(event);
			break;
		}
	}

	// キーボード入力更新
	INPUT_INSTANCE.Update();

	// コントローラ入力更新
	CONTROLLER_INSTANCE.Update();

	// ステアリングコントローラ入力更新
	STEERING_CONTROLLER_INSTANCE.Update();

	// マウス入力更新
	MOUSE_INSTANCE.Update();

	// ESCが押されたら終了
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_ESCAPE))
	{
		m_isRunning = false;
	}

	// アクターデバッグ
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_F12))
	{
		ShowActor();
	}

	// レンダリングリソース表示
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_F11))
	{
		GetRenderer()->ShowResource();
	}

	// ポーズモード移行／解除
	if (INPUT_INSTANCE.IsKeyPullUp(SDL_SCANCODE_BACKSPACE) || CONTROLLER_INSTANCE.IsReleased(SDL_CONTROLLER_BUTTON_START))
	{
		//ポーズモード切替
		m_isPauseMode = !m_isPauseMode;
		Actor::State changeState;
		if (m_isPauseMode)
		{
			changeState = Actor::STATE_PAUSED;        // ポーズ
			m_pause->SetModeON();                     // ポーズ画面をオン
		}
		else
		{
			m_pause->SetModeOFF();                    // ポーズ画面をオフ
			changeState = Actor::STATE_ACTIVE;        // アクティブ
		}

		//全てのステートを変更する
		for (auto itr : m_actors)
		{
			if (itr->GetState() != Actor::STATE_DEAD)
			{
				itr->SetState(changeState);
			}
		}
	}
}

void GameMain::SetDrawDebugBox(OBB in_box)
{
	if (!m_debugBox)
	{
		return;
	}

	m_debugBox->SetDebugBox(in_box);
}

void GameMain::DrawDebugBoxes()
{
	if (!m_debugBox)
	{
		return;
	}

	m_debugBox->Draw();
}

SDL_Renderer* GameMain::GetSDLRenderer()
{
	return m_renderer->GetSDLRenderer();
}

// アクターの追加処理
void GameMain::AddActor(Actor * in_actor)
{
	// ペンディングアクター側に追加
	m_pendingActors.emplace_back(in_actor);
}

// アクターの削除処理
void GameMain::RemoveActor(Actor * in_actor)
{
	// ペンディングアクター内にいるか
	auto iter = std::find(m_pendingActors.begin(), m_pendingActors.end(), in_actor);
	// イテレーターがペンディングアクターの末尾に来たら削除してよい
	if (iter != m_pendingActors.end())
	{
		// ペンディングアクターの末尾にデータを移し、そのデータをpop_back消去
		std::iter_swap(iter, m_pendingActors.end() - 1);
		m_pendingActors.pop_back();
		// 終了
		return;
	}

	// アクティブアクター内にいるか
	iter = std::find(m_actors.begin(), m_actors.end(), in_actor);

	if (iter != m_actors.end())
	{
		// アクティブアクターの末尾にデータを移し、そのデータをpop_back消去
		std::iter_swap(iter, m_actors.end() - 1);
		m_actors.pop_back();
	}
}

void GameMain::DeadAllActor()
{
	for (auto actor : m_actors)
	{
		actor->SetState(Actor::STATE_DEAD);
	}
}

// UI画面の追加
void GameMain::AddUI(UIScreen * in_screen)
{
	m_uiStack.emplace_back(in_screen);
}

// UIの反転(ポーズ画面を最前面にするための処理)
void GameMain::SwapPauseUI()
{
	m_pause->Close();

	m_pause = new PauseScreen();
}

// 引数のキーとなるファイルパスでフォント配列を検索し、一致したフォントを返す
// 見つからなかった場合、新規にフォントを生成し、配列に追加後、フォントを返す
Font* GameMain::GetFont(const std::string & in_keyPath)
{
	// 同じフォントがすでに配列にないか検索
	auto iter = m_fonts.find(in_keyPath);

	// 同じフォントが入っていたら
	if (iter != m_fonts.end())
	{
		// 同じフォントを返す
		return iter->second;
	}
	// フォントが入っていなかったらフォントを生成し、生成したフォントを返す
	else
	{
		Font* font = new Font();

		if (font->Load(in_keyPath))
		{
			m_fonts.emplace(in_keyPath, font);
		}
		else
		{
			font->UnLoad();
			delete font;

			font = nullptr;
		}
		return font;
	}


}

// テキストのロード
void GameMain::LoadText(const std::string & in_fileName)
{
	// テキストマップのクリア
	m_text.clear();
	// 指定パスのファイルを受け取る
	std::ifstream file(in_fileName);
	// 開けなかったら
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found\n", in_fileName.c_str());
		return;
	}

	// 文字列を取得
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	// rapidjsonで開く
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON\n", in_fileName.c_str());
		return;
	}
	// テキストマップに保管
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			m_text.emplace(itr->name.GetString(),
				itr->value.GetString());
		}
	}
}

// テキストの取得
const std::string & GameMain::GetText(const std::string & in_key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	//マップから指定のキーを検索
	auto iter = m_text.find(in_key);
	if (iter != m_text.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}

// カメラのセット
void GameMain::SetCamera(Camera * in_camera)
{

	//printf("SetCamera [%p]\n", in_camera);
	//m_activeCamera = in_camera;

}

void GameMain::SetCamera(CameraComponent* in_camera)
{
	//printf("SetCamera [%p]\n", in_camera);
	m_activeCamera = in_camera;
}

// 引数のカメラをアクティブにする(レンダラーのカメラを登録)
void GameMain::InActiveCamera(Camera * in_activeCam)
{
	//if (in_activeCam == m_activeCamera)
	//{
	//	printf("Camera is inActive, change to default view.\n");
	//	m_activeCamera = nullptr;
	//}

	//m_viewMatrix = 
	//	Matrix4::CreateLookAt(Vector3(0, 0, 100),
	//	Vector3(0, 0, 0),
	//	Vector3(0, 0, 1));
}

void GameMain::InActiveCamera(CameraComponent* in_activeCam)
{
	if (in_activeCam == m_activeCamera)
	{
		printf("Camera is inActive, change to default view.\n");
		m_activeCamera = nullptr;
	}

	m_viewMatrix =
		Matrix4::CreateLookAt(Vector3(0, 0, 100),
			Vector3(0, 0, 0),
			Vector3(0, 0, 1));
}

const Vector3 & GameMain::GetViewVector()
{
	if (m_activeCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
		return Vector3::Zero;
	}
	return m_activeCamera->GetViewMatrix().GetTranslation();

}



void GameMain::ShowActor()
{
	printf("\n\n<--------------ActorList----------------->\n");
	printf("---------> Active Actor ( %zd ) <-----------\n", m_actors.size());

	for (auto i : m_actors)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}
	printf("---------> Pending Actor ( %zd ) <-----------\n", m_pendingActors.size());
	for (auto i : m_pendingActors)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}
}

void GameMain::Draw()
{


	if (!m_nowScene)
	{
		return;
	}


	m_nowScene->Draw();

	

}

// ゲームループ制御用のヘルパー関数
void GameMain::LoopHelper()
{
	SDL_Event event;
	// キューにイベントがあれば繰り返す
	switch (SDL_PollEvent(&event))
	{
		// ウィンドウを閉じた(xをクリックした)場合、メインループを終了
		case SDL_QUIT:
			m_isRunning = false;
			break;
	}

	// キーボードの押下状態を取得
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	// ESCキーでメインループを抜ける
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		m_isRunning = false;
	}
}

void GameMain::DebugRenderer()
{
	// SDLウィンドウの塗りつぶし(デバッグ用)
	SDL_SetRenderDrawColor
	(
		m_debugRenderer,
		0,                   // R
		0,                   // G
		255,                 // B
		255                  // A
	);

	// バックバッファを現在の描画色でクリア
	SDL_RenderClear(m_debugRenderer);



	// フロントバッファとバックバッファを交換
	SDL_RenderPresent(m_debugRenderer);
}
