#include "MissionManager.h"
#include "GameWorld.h"
#include "MissionBase.h"
#include "MissionUI.h"
#include "ClientActor.h"
#include "Input.h"
#include "InputController.h"
#include <stdio.h>
#include <time.h>

const int MissionManager::MISSION_ALL_NUM = 3;    // �����i�s����C���̌��E��

// �R���X�g���N�^
MissionManager::MissionManager(GameWorld* in_world)
	:m_player(in_world->GetPlayer())
	,m_world(in_world)
	,m_selectNum(0)
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


	// �~�b�V�����J�[�\��
	ChangeSelectNum();

	
}

void MissionManager::InitRandom()
{
	srand((unsigned int)time(NULL));
}

void MissionManager::ChangeSelectNum()
{
	// �J�[�\�����ړ�
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
	{
		m_selectNum++;
		// �~�b�V�������𒴂�����0�Ԗڂɐ܂�Ԃ�
		if (m_selectNum >= MISSION_ALL_NUM)
		{
			m_selectNum = 0;
		}
	}
	
	// �J�[�\����ړ�
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_DPAD_UP))
	{
		m_selectNum--;
		// 1��������܂�Ԃ�
		if (m_selectNum < 0)
		{
			m_selectNum = 2;
		}
	}

	for (auto mission : m_missions)
	{
		
	}
}
