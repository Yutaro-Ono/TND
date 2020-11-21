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

	// ミッションの目的別
	typedef enum MISSION_TYPE
	{
		DELIVERY,     // 荷物配達
		TAXI          // タクシー(人を乗せる)
	};
	// ミッションのステート
	typedef enum MISSION_STATE
	{
		HOLD,            // 保留状態
		ACTIVE,          // アクティブ
		SUCCESS,         // 成功
		FAILED           // 失敗
	};


	MissionBase(class MissionManager* in_manager, MISSION_TYPE in_type, int in_listNum);
	~MissionBase();

	// 更新処理
	void Update(float in_deltaTime);

	// ミッション詳細内容の定義
	void SetMissionDetail(class ClientActor* in_start, class ClientActor* in_goal, unsigned int in_baseScore, unsigned int in_timeLimit);

	// プレイヤーの座標～開始地点or終了地点座標の距離を求め、一定以上接近してボタンを押したら真)
	void CheckDistPlayer(const Vector3& in_playerPos, const Vector3& in_missionPos);

	// 耐久値減少処理
	void DecraseDurableValue();

	//------------------------------------------------------------+
	// Getter / Setter
	//------------------------------------------------------------+
	// ミッション状態のセッター・ゲッター
	void SetMissionState(MISSION_STATE in_state) { m_missionState = in_state; }
	MISSION_STATE GetMissionState() { return m_missionState; }
	// 開始地点アクターのゲッター・ゴール地点アクターのゲッター
	class ClientActor* GetStartActor() { return m_startActor; }
	class ClientActor* GetGoalActor() { return m_goalActor; }
	// ミッションタイプのセッターゲッター
	MISSION_TYPE GetMissionType() { return m_missionType; }
	// プレイヤーとの距離のゲッター
	float GetPlayerDistance() { return m_playerDistance; }

	// 制限時間取得
	unsigned int GetTimeLimit() { return m_timeLimit; }

	// 耐久値取得
	unsigned int GetDurableValue() { return m_durableVal; }
	// リスト番号のゲッター
	int GetListNum() { return m_listNum; }
	void SetListNum(int in_listNum) { m_listNum = in_listNum; }

	// ミッションマネージャーのゲッター
	class MissionManager* GetMissionManager() { return m_manager; }

	// スコアの計算・ゲッター
	int GetCalcScore();
	int GetBaseScore() { return m_baseScore; }

protected:


	MISSION_STATE m_missionState;     // ミッションのステート
	MISSION_TYPE m_missionType;       // ミッションタイプ

	class Actor* m_client;           // 依頼者アクター
	class MissionUI* m_missionUI;    // ミッションの内容を表示するUIクラス
	class MissionManager* m_manager; // ミッションマネージャー

	// 目標定義
	class ClientActor* m_startActor;  // 出発地点となるアクター
	class ClientActor* m_goalActor;   // ゴール地点となるアクター
	Vector3 m_startPos;              // 出発地点(依頼者の位置)
	Vector3 m_goalPos;               // ゴール地点(配送先の位置)
	float m_playerDistance;          // プレイヤーまでの距離


	unsigned int m_durableVal;       // 耐久値

	unsigned int m_baseScore;        // スコア

	// 制限時間関連
	unsigned int m_timeLimit;                   // 制限時間
	unsigned int m_lastTime, m_currentTime;     // 時間計測用の現在時刻と最終更新時刻


	int m_listNum;                           // ミッションリストの何番目か
};