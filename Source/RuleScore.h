//-----------------------------------------------------------------------+
// スコアクラス
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Actor.h"


class RuleScore : public Actor
{


public:

	RuleScore(class Player* in_target);                                            // コンストラクタ
	~RuleScore();                                                                  // デストラクタ

	void UpdateActor(float in_deltaTime) override;                                 // 更新処理

	void AddScore(float in_scoreRate);                                             // スコアの加算

	const int& GetScore() const { return m_score; }                                // スコアのゲッター

	const int& GetChain() const { return m_chain; }                                // チェインのゲッター

	const float& GetChainTimer() const { return m_chainTimer; }                    // チェインタイマーのゲッター

	const float& GetChainTimerLimit() const { return m_chainTimerLimit; }          // チェインタイマーの制限時間の値を返す

	const float& GetChainRate() const { return m_chainRate; }                      // チェイン数によるスコア倍率を返す

	const int& GetPlayerRank() const { return m_playerRank; }                      // ともし火取得数によるプレイヤーランクの取得


private:


	class Player* m_target;                                         // ターゲットするプレイヤー

	int m_score;                                                    // スコア

	int m_chain;                                                    // チェイン数

	float m_chainRate;                                              // チェイン数によるスコア倍率

	float m_chainTimer;

	float m_chainTimerLimit;

	float m_scoreRate;                                              // スコアの倍率

	float m_playerAccelLimit;                                       // プレイヤーのアクセル限界値

	int m_playerRank;

	static const int SCORE_POINT;                                   // スコアの加算基準値
};