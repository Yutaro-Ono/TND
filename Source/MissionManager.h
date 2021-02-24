#pragma once
#include <vector>
#include <string>
#include "Math.h"

class MissionManager
{

public:


	MissionManager(class GameWorld* in_world);
	~MissionManager();

	void Update(float in_deltaTime);
	void InitRandom();
	void ChangeSelectNum();                  // 何番目のミッションを指しているか

	float CalcScoreForDistance(const Vector3& in_pPos, const Vector3& in_cPos1, const Vector3& in_cPos2);     // 距離に応じたミッションごとのスコア算出

	//------------------------------------------+
	// Getter / Setter
	//------------------------------------------+
	const std::vector<class MissionBase*>& GetMissionList() const { return m_missions; }     // ミッションリストのゲッター
	class PlayerManager* GetPlayer() { return m_player; }

	// カーソルで選択中のミッション番号ゲッター
	int GetSelectedMission() { return m_selectNum; }


private:

	class PlayerManager* m_player;                // プレイヤーへのポインタ

	class GameWorld* m_world;                     // ワールドへのポインタ

	std::vector<class MissionBase*> m_missions;   // 任務スタック

	int m_selectNum;                              // 選択しているミッションNo


	static const int MISSION_ALL_NUM;             // 同時進行する任務の限界数

	std::string m_selectSE;                       // カーソル選択変更時の効果音

};