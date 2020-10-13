#pragma once
#include <list>


class MissionManager
{

public:

	MissionManager(class GameWorld* in_world);
	~MissionManager();


private:


	std::list<class MissionBase*> m_missions;     // 任務スタック



	static const int MISSION_ALL_NUM;             // 同時進行する任務の限界数

};