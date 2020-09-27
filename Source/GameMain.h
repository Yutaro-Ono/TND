//-----------------------------------------------------------------------+
// ゲームループ.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
// インクルードファイル
#include <unordered_map>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_types.h>
#include "Math.h"

class SceneBase;
class TestScene;

class GameMain
{


public:

	// ゲームの状態管理
	enum GameState
	{
		PLAYING,
		PAUSED,
		QUIT
	};

	// シングルトン
	static GameMain& GetInstance()
	{
		static GameMain game;
		return game;
	}

	~GameMain();                                                                    // デストラクタ

	bool   Initialize(int in_screenW, int in_screenH, bool in_full);                // 各種初期化処理
	void   Delete();                                                                // 各種解放処理

	void   RunLoop();                                                               // ゲームのメインループ
	void   CloseGame();                                                             // ゲームの終了処理

	void   SetFirstScene(class SceneBase* in_scene);                                // 開始シーンのセット

	void SetShutDown() { m_isRunning = false; }                                     // シャットダウン処理



	void AddActor(class Actor* in_actor);                                           // アクターの追加
	void RemoveActor(class Actor* in_actor);                                        // アクターの削除
	const std::vector<class Actor*>& GetActorStack() const { return m_actors; }     // アクター配列の取得
	void AllDeadActor();                                                            // アクター全削除


    //--------------------------------------------------------------------//
	// Getter/Setter
	//-------------------------------------------------------------------//
	class Renderer* GetRenderer() const { return m_renderer; }                       // レンダラー取得
	SDL_Renderer* GetSDLRenderer();                                                  // 2D用SDLレンダラーの取得
	// カメラ
	void SetCamera(class Camera* in_camera);                                         // カメラをシステムに登録
	void InActiveCamera(class Camera* in_activeCam);                                 // カメラの登録を解除
	// 行列取得
	const Matrix4& GetViewMatrix() { return m_viewMatrix; };                         // ビュー行列のゲッター
	const Vector3& GetViewVector();                                                  // ビュー座標のゲッター
	// フレーム計測
	const int GetFrame() { return m_frame; }                                         // フレームのゲッター
	// デルタタイム取得
	const float GetDeltaTime() { return m_deltaTime; }                               // デルタタイムの取得
	// 当たり判定
	class PhysicsWorld* const GetPhysics() { return m_physicsWorld; }                
	// デバッグ用視覚化ボックス
	void SetDrawDebugBox(struct OBB in_box);
	void DrawDebugBoxes();
	// オーディオ
	class AudioManager* const GetAudio() { return m_audio; }                          // オーディオのゲッター
	// UI
	const std::vector<class UIScreen*>& GetUIStack() { return m_uiStack; }            // UIスタックのゲッター
	void AddUI(class UIScreen* in_screen);
	void SwapPauseUI();                                                               // ポーズ画面を最前面に持ってくる
	// フォント
	class Font* GetFont(const std::string& in_fileName);
	void LoadText(const std::string& in_fileName);
	const std::string& GetText(const std::string& in_key);
	// ロード画面クラスのゲッター
	class LoadScreen* const GetLoadScreen() { return m_load; }


private:

	GameMain();                                                              // コンストラクタ

	void   Load();                                                           // データのロード
	void   Unload();                                                         // データのアンロード

	void Input();                                                            // 入力処理

	int   UpdateGame();                                                      // ゲームの更新処理
	void UpdateActor();                                                      // アクターの更新処理

	void ShowActor();                                                        // アクターリスト表示(デバッグ用)

	void Draw();                                                             // 描画処理

	// ゲームループ制御と描画用のヘルパー関数
	void LoopHelper();
	void DebugRenderer();

	GameState    m_state;                                                    // ゲームの状態(プレイ中かポーズ中か終了か)

	bool m_isRunning;                                                        // ゲームループを回し続ける(続行する)かどうか
	bool m_isPauseMode;                                                      // ポーズモード

	int          m_frame;                                                    // フレーム計測用(パーティクルの生成に用いる)
	float        m_deltaTime;                                                // 1フレームの経過時間(秒)
	Uint32       m_ticksCount;                                               // ミリ秒単位の経過時間
	Matrix4      m_viewMatrix;

	class Renderer* m_renderer;                                              // レンダリングクラス
	class SceneBase*   m_nowScene;                                           // 現在のシーンのインスタンス
	class Camera* m_activeCamera;                                            // アクティブなカメラ (システムが描画に用いる)
	class PhysicsWorld* m_physicsWorld;                                      // 当たり判定システム
	class DebugBox* m_debugBox;                                              // デバッグ用ボックス
	class AudioManager* m_audio;                                             // オーディオ管理

	class PauseScreen* m_pause;                                              // ポーズ画面
	class LoadScreen* m_load;                                                // ロード画面

	std::vector<class Actor*> m_actors;                                      // アクター配列
	std::vector<class Actor*> m_pendingActors;                               // アクター追加準備用配列

	std::vector<class UIScreen*> m_uiStack;                                  // UIスタック
	std::unordered_map<std::string, class Font*> m_fonts;                    // フォント
	std::unordered_map<std::string, std::string> m_text;


	SDL_Window* m_debugWindow;                                               // SDLウィンドウ(デバッグ用)
	SDL_Renderer* m_debugRenderer;                                           // SDLレンダラー


};

// シングルトンのゲッター
#define GAME_INSTANCE GameMain::GetInstance()
#define RENDERER GameMain::GetInstance().GetRenderer()
#define AUDIO GameMain::GetInstance().GetAudio()