#include "MissionBase.h"
#include "Actor.h"
#include "MissionUI.h"

MissionBase::MissionBase(Actor* in_client)
	:m_client(in_client)
	,m_missionState(HOLD)
{
	// 初期化
	m_startPos = m_goalPos = Vector3::Zero;
	m_currentTime = m_lastTime = SDL_GetTicks();

	// UIの生成
	m_missionUI = new MissionUI(this);
}

MissionBase::~MissionBase()
{
}

// 更新処理
void MissionBase::Update(float in_deltaTime)
{

	//---------------------------------------------------+
	//
	// ステートごとの更新処理
	//
	//---------------------------------------------------+
	// ミッション保留中
	if (m_missionState == HOLD)
	{
		// 最終時刻を更新し続ける
		m_lastTime = SDL_GetTicks();
	}

	// ミッション受注時
	if (m_missionState == ACTIVE)
	{

		// 現在時刻を更新
		m_currentTime = SDL_GetTicks();

		//-----------------------------------------+
		// カウントダウン処理
		//-----------------------------------------+
		// 一秒経過後
		if (m_currentTime > m_lastTime + 1000)
		{
			// 制限時間を1減らす
			m_timeLimit -= 1;
			// 最終時刻更新
			m_lastTime = m_currentTime;
		}

		// 制限時間が切れたらミッション失敗
		if (m_timeLimit <= 0)
		{
			// UIを落とす
			m_missionUI->Close();
			// 失敗状態にする
			m_missionState = FAILED;
		}
	}




}


// ミッション詳細設定の定義
void MissionBase::SetMissionDetail(const Vector3& in_start, const Vector3& in_goal, unsigned int in_baseScore, unsigned int in_timeLimit)
{
	m_startPos = in_start;          // 開始地点
	m_goalPos = in_goal;            // ゴール地点

	m_baseScore = in_baseScore;     // ベーススコア
	m_timeLimit = in_timeLimit;     // 制限時間
}

