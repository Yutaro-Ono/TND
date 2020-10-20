#pragma once
#include <vector>


class MissionManager
{

public:


	MissionManager(class GameWorld* in_world);
	~MissionManager();

	void Update(float in_deltaTime);

	void InitRandom();

	//------------------------------------------+
	// Getter / Setter
	//------------------------------------------+
	const std::vector<class MissionBase*>& GetMissionList() const { return m_missions; }     // ミッションリストのゲッター

	class PlayerManager* GetPlayer() { return m_player; }

private:

	class PlayerManager* m_player;                // プレイヤーへのポインタ

	class GameWorld* m_world;                     // ワールドへのポインタ

	std::vector<class MissionBase*> m_missions;     // 任務スタック

	static const int MISSION_ALL_NUM;             // 同時進行する任務の限界数

};