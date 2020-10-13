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

	MissionBase();
	~MissionBase();

	void SetPosition(const Vector3& in_start, const Vector3& in_goal);


protected:


	// 目標定義
	Vector3 m_startPos;          // 出発地点(依頼者の位置)
	Vector3 m_goalPos;           // ゴール地点(配送先の位置)

	unsigned int m_baseScore;    // スコア

	unsigned int m_timeLimit;    // 制限時間
};