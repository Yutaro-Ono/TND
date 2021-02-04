//-----------------------------------------------------------------------+
// ゲームシーンクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

#include "SceneBase.h"
#include <unordered_map>


class GameScene : public SceneBase
{


public:


	enum SceneState
	{
		STATE_FADE_IN,
		STATE_FADE_OUT,
		STATE_FREE,
		STATE_START,
		STATE_IN_GAME,
		STATE_FINISH
	};



	GameScene(int in_stageNum);                               // コンストラクタ
	~GameScene();                                             // デストラクタ


	void Initialize() override;                               // 初期化処理

	SceneBase* Update() override;                             // 更新処理

	void Draw() override;                                     // 描画処理

	SceneState GetState() { return m_state; }                 // ステートのゲッター

	void Wait(int in_millisec);                               // 指定した秒だけウェイト

	void Loading();                                           // ロード画面演出


private:

	int m_stageNum;                                           // ステージ番号

	class GameWorld* m_world;                                 // ゲームワールドクラス

	class RuleTime* m_time;
	class RuleScore* m_score;                                 // スコア

	int m_prevScore;                                          // 更新前のスコア
	int m_nowScore;                                           // 毎フレームのスコア

	float m_bestSpeed;                                        // プレイヤーが達成した最高速度

	std::unordered_map<std::string, std::string> m_sound;     // 効果音

	SceneState m_state;                                       // ステート

	float m_cameraMove;                                       // カメラの移動量

};