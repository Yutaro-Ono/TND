#include "MissionManager.h"
#include "GameWorld.h"
#include "MissionBase.h"
#include "MissionUI.h"


const int MissionManager::MISSION_ALL_NUM = 3;    // �����i�s����C���̌��E��

// �R���X�g���N�^
MissionManager::MissionManager(GameWorld* in_world)
{

	// �~�b�V�������X�g
	for (int i = 0; i < MISSION_ALL_NUM; i++)
	{
		//m_missions.push_back(new MissionBase(in_world));
	}
}

// �f�X�g���N�^
MissionManager::~MissionManager()
{
}
