#pragma once
#include "GameMain.h"

//-------------------------------------------------+
//
// 任務ベースクラス
//
//-------------------------------------------------+


class MissionBase
{

public:

	// ミッションのステート
	typedef enum MissionState
	{
		HOLD,            // 保留状態
		ACTIVE,          // アクティブ
		SUCCESS,         // 成功
		FAILED           // 失敗
	};


	MissionBase(class Actor* in_client);
	~MissionBase();

	// 更新処理
	void Update(float in_deltaTime);

	// ミッション詳細内容の定義
	void SetMissionDetail(const Vector3& in_start, const Vector3& in_goal, unsigned int in_baseScore, unsigned int in_timeLimit);

	// ミッション状態のセッター・ゲッター
	void SetMissionState(MissionState in_state) { m_missionState = in_state; }
	MissionState GetMissionState() { return m_missionState; }


protected:


	MissionState m_missionState;     // ミッションのステート

	class Actor* m_client;           // 依頼者アクター

	class MissionUI* m_missionUI;    // ミッション概要表示UI

	// 目標定義
	Vector3 m_startPos;              // 出発地点(依頼者の位置)
	Vector3 m_goalPos;               // ゴール地点(配送先の位置)

	unsigned int m_baseScore;        // スコア

	// 制限時間関連
	unsigned int m_timeLimit;                   // 制限時間
	unsigned int m_lastTime, m_currentTime;     // 時間計測用の現在時刻と最終更新時刻

};