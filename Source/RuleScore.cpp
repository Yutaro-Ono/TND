//-----------------------------------------------------------------------+
// スコアクラス
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "RuleScore.h"


const int RuleScore::SCORE_POINT = 100;                // 加算されるスコア用ポイントの基準値

// コンストラクタ
RuleScore::RuleScore(Player * in_target)
	:m_target(in_target)
	,m_score(0)
	,m_chain(0)
	,m_chainRate(0.0f)
	,m_chainTimer(0.0f)
	,m_chainTimerLimit(0.0f)
	,m_playerRank(0)
	,m_scoreRate(1.0f)
{
	//m_playerAccelLimit = m_target->GetAccelLimit();

	//m_chainTimerLimit = m_target->GetChainTimerLimit();

	//m_playerRank = m_target->GetSpeedRank();
}

// デストラクタ
RuleScore::~RuleScore()
{
}

// 更新処理
void RuleScore::UpdateActor(float in_deltaTime)
{
	//// プレイヤーのアクセルの限界値を更新
	//float prevPlayerAccelLimit = m_target->GetAccelLimit();

	//// タイマーの取得
	//m_chainTimer = m_target->GetChainTimer();

	//// プレイヤーが炎を取得し加速していたら
	//if (m_playerAccelLimit != prevPlayerAccelLimit)
	//{
	//	AddScore(m_scoreRate);
	//	m_playerAccelLimit = prevPlayerAccelLimit;
	//}

	////--------------------------------------------------------+
	//// チェインの更新
	////--------------------------------------------------------+
	//int nowChain = m_target->GetFlameChain();

	//// チェイン数の更新
	//if (m_chain != nowChain)
	//{
	//	m_chain = nowChain;

	//	// チェイン数に応じたスコアレート(50チェイン時、約2倍に)
	//	m_chainRate = 1.0f + ((m_chain - 1) * 0.125);
	//}

	//// チェインレートの正規化
	//if (m_chain == 0)
	//{
	//	m_chainRate = 1.0f;
	//}

	//// スコアレートをプレイヤーの速度に応じて更新
	//m_scoreRate = m_target->GetAccel() / m_target->GetAccelMax();

	////--------------------------------------------------------+
	//// ランクの更新
	////--------------------------------------------------------+
	//int m_nowRank = m_target->GetSpeedRank();

	//if (m_playerRank != m_nowRank)
	//{
	//	// ランク更新
	//	m_playerRank = m_nowRank;
	//}


}

// スコアの加算
void RuleScore::AddScore(float in_scoreRate)
{
	m_score += SCORE_POINT * m_chainRate;
}
