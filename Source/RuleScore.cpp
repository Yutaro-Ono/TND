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

}

// デストラクタ
RuleScore::~RuleScore()
{
}

// 更新処理
void RuleScore::UpdateActor(float in_deltaTime)
{


}

// スコアの加算
void RuleScore::AddScore(float in_scoreRate)
{
	m_score += SCORE_POINT * m_chainRate;
}
