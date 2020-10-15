#include "MissionManager.h"
#include "GameWorld.h"
#include "MissionBase.h"
#include "MissionUI.h"


const int MissionManager::MISSION_ALL_NUM = 3;    // 同時進行する任務の限界数

// コンストラクタ
MissionManager::MissionManager(GameWorld* in_world)
{

	// ミッションリスト
	for (int i = 0; i < MISSION_ALL_NUM; i++)
	{
		//m_missions.push_back(new MissionBase(in_world));
	}
}

// デストラクタ
MissionManager::~MissionManager()
{
}
