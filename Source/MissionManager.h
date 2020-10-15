#pragma once
#include <list>


class MissionManager
{

public:


	MissionManager(class GameWorld* in_world);
	~MissionManager();


	//------------------------------------------+
	// Getter / Setter
	//------------------------------------------+
	const std::list<class MissionBase*>& GetMissionList() const { return m_missions; }     // ミッションリストのゲッター


private:


	std::list<class MissionBase*> m_missions;     // 任務スタック

	static const int MISSION_ALL_NUM;             // 同時進行する任務の限界数

};