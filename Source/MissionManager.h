#pragma once
#include <vector>


class MissionManager
{

public:


	MissionManager(class GameWorld* in_world);
	~MissionManager();

	void Update(float in_deltaTime);
	void InitRandom();
	void ChangeSelectNum();                  // 何番目のミッションを指しているか

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

	

};