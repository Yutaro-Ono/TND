//-----------------------------------------------------------------------+
// リザルトシーンクラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once

// インクルードファイル
#include "SceneBase.h"
#include <unordered_map>
#include "AudioManager.h"

class ResultScene : public SceneBase
{


public:
	
	enum ResultState
	{
		STATE_WHITE_OUT,
		STATE_WHITE_IN,
		STATE_SCORE_DISPLAY,
		STATE_ONE_MORE,
		STATE_GAME_QUIT
	};

	ResultScene(int in_score, float in_bestSpeed);                               // コンストラクタ
	~ResultScene();                                          // デストラクタ

	void Initialize() override;                              // 初期化処理

	SceneBase* Update() override;                            // シーンの更新処理

	void Draw() override;                                    // 描画処理

	ResultState GetResultState() { return m_state; }


private:

	int m_score;                                              // スコア

	float m_bestSpeed;                                         // ゲームシーン中に達成した最高速度

	ResultState m_state;                                      // リザルト画面のステート

	class Environment* m_environment;                         // 環境クラス

	class ResultCar* m_car;
	class BridgeObject* m_bridge[8];

	class ResultScreen* m_resultUI;                           // リザルトシーンUI

	class RecordScore* m_record;

	std::unordered_map<std::string, std::string> m_sound;     // 音楽


};