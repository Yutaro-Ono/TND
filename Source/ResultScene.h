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


class ResultScene : public SceneBase
{


public:
	
	enum ResultState
	{
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

	class Skydome* m_skydome;                                 // スカイドーム

	class FlameStatue* m_flameStatue;                              // 燭台

	class AngelStatue* m_monkStatue[3];

	class FlameActor* m_flame;                                // ともし火(スコア)

	class ResultScreen* m_resultUI;                           // リザルトシーンUI

	class RecordScore* m_record;

	std::unordered_map<std::string, std::string> m_sound;     // 音楽


};