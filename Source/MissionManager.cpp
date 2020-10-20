#include "MissionManager.h"
#include "GameWorld.h"
#include "MissionBase.h"
#include "MissionUI.h"
#include "ClientActor.h"
#include <stdio.h>
#include <time.h>

const int MissionManager::MISSION_ALL_NUM = 3;    // �����i�s����C���̌��E��

// �R���X�g���N�^
MissionManager::MissionManager(GameWorld* in_world)
	:m_player(in_world->GetPlayer())
	,m_world(in_world)
{
	InitRandom();
	// �~�b�V�������X�g�̐���
	for (int i = 0; i < MISSION_ALL_NUM; i++)
	{
		// �~�b�V������ނ̗���
		int missionType = rand() % 2 + 1;
		// �˗��l�ԍ��̗���
		int startPos = rand() % m_world->GetClients().size();
		int goalPos = rand() % m_world->GetClients().size();

		

		// �~�b�V�����^�C�v���ƂɃ~�b�V�������e��ݒ�
		if (missionType == 1)
		{
			// �z�B�~�b�V�����̒ǉ�
			m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
			// �~�b�V�����T�v�Z�b�g
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos]->GetPosition(),
				m_world->GetClients()[goalPos]->GetPosition(), 1000, 30);
		}
		else
		{
			// �쑗�~�b�V�����̒ǉ�
			m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
			// �~�b�V�����T�v�Z�b�g
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos]->GetPosition(),
				m_world->GetClients()[goalPos]->GetPosition(), 1000, 30);
		}
	}
}

// �f�X�g���N�^
MissionManager::~MissionManager()
{
	while (!m_missions.empty())
	{
		delete m_missions.back();
	}
}

void MissionManager::Update(float in_deltaTime)
{
	
	// ���X�g�ԍ��X�V�p
	int listNum = 0;
	// �I�������~�b�V�������~�b�V�������X�g����폜����
	std::vector<MissionBase*> endMissions;
	for (auto mission = m_missions.begin(); mission != m_missions.end();)
	{
		(*mission)->SetListNum(listNum);
		(*mission)->Update(in_deltaTime);
		listNum++;

		// �~�b�V�������s���܂��͐������ɍ폜�p�z���
		if ((*mission)->GetMissionState() == MissionBase::FAILED || (*mission)->GetMissionState() == MissionBase::SUCCESS)
		{
			endMissions.emplace_back(*mission);
			mission = m_missions.erase(mission);
		}
		else
		{
			++mission;
		}
	}
	// �폜����
	for (auto end : endMissions)
	{
		delete end;
	}
	endMissions.clear();

	// �~�b�V�������X�g�͏펞3�B�����Ă�����쐬����
	int addMissionNum = MISSION_ALL_NUM - m_missions.size();
	if (addMissionNum > 0)
	{

		for (int i = 0; i < addMissionNum; i++)
		{
			InitRandom();
			// �~�b�V������ނ̗���
			int missionType = rand() % 2 + 1;
			// �˗��l�ԍ��̗���
			int startPos = rand() % m_world->GetClients().size();
			int goalPos = rand() % m_world->GetClients().size();

			// �~�b�V�����^�C�v���ƂɃ~�b�V�������e��ݒ�
			if (missionType == 1)
			{
				// �z�B�~�b�V�����̒ǉ�
				m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
				// �~�b�V�����T�v�Z�b�g
				m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos]->GetPosition(),
					m_world->GetClients()[goalPos]->GetPosition(), 1000, 30);
			}
			else
			{
				// �쑗�~�b�V�����̒ǉ�
				m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
				// �~�b�V�����T�v�Z�b�g
				m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos]->GetPosition(),
					m_world->GetClients()[goalPos]->GetPosition(), 1000, 30);
			}
		}
	}



	
}

void MissionManager::InitRandom()
{
	srand((unsigned int)time(NULL));
}
