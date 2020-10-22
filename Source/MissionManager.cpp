#include "MissionManager.h"
#include "GameWorld.h"
#include "MissionBase.h"
#include "MissionUI.h"
#include "ClientActor.h"
#include "Input.h"
#include "InputController.h"
#include "ScoreUI.h"
#include <stdio.h>
#include <time.h>

const int MissionManager::MISSION_ALL_NUM = 3;    // �����i�s����C���̌��E��


// �R���X�g���N�^
MissionManager::MissionManager(GameWorld* in_world)
	:m_player(in_world->GetPlayer())
	,m_world(in_world)
	,m_scoreUI(nullptr)
	,m_selectNum(0)
{
	InitRandom();

	// �~�b�V�����J�n�n�_�ƏI���n�_
	// �����Ő������A�J�n�E�I���_�Ŕ�肪�Ȃ������̃~�b�V�����Ɣ��Ȃ�����
	int startPos[MISSION_ALL_NUM], goalPos[MISSION_ALL_NUM];
	
	// �~�b�V�������X�g�̐���
	for (int i = 0; i < MISSION_ALL_NUM; i++)
	{
		// �~�b�V������ނ̗���
		int missionType = rand() % 2 + 1;
		// �˗��l�ԍ��̗���
		startPos[i] = rand() % m_world->GetClients().size();
		goalPos[i] = rand() % m_world->GetClients().size();

		// ���W�̔�肪�Ȃ��悤�ɂ���
		do
		{
			startPos[i] = rand() % m_world->GetClients().size();
		} while (m_world->GetClients()[startPos[i]]->GetIsSelected());
		m_world->GetClients()[startPos[i]]->SetIsSelected(true);
		do
		{
			goalPos[i] = rand() % m_world->GetClients().size();

		} while (m_world->GetClients()[goalPos[i]]->GetIsSelected());
		m_world->GetClients()[goalPos[i]]->SetIsSelected(true);
		


		// �~�b�V�����^�C�v���ƂɃ~�b�V�������e��ݒ�
		if (missionType == 1)
		{
			// �z�B�~�b�V�����̒ǉ�
			m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
			// �~�b�V�����T�v�Z�b�g
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos[i]],
				m_world->GetClients()[goalPos[i]], 1000, 45);
		}
		else
		{
			// �쑗�~�b�V�����̒ǉ�
			m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
			// �~�b�V�����T�v�Z�b�g
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos[i]],
				m_world->GetClients()[goalPos[i]], 1000, 45);
		}
	}


	// �X�R�AUI�𐶐�
	m_scoreUI = new ScoreUI();
}

// �f�X�g���N�^
MissionManager::~MissionManager()
{
	while (!m_missions.empty())
	{
		delete m_missions.back();
	}
}

// �X�V����
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

		// �~�b�V�������s���܂��͐�����
		// �X�R�A�����Z (���s����0)
		// �폜�p�z���
		if ((*mission)->GetMissionState() == MissionBase::FAILED || (*mission)->GetMissionState() == MissionBase::SUCCESS)
		{
			// �X�R�AUI�ɃX�R�A���Z�b�g
			m_scoreUI->AddScore((*mission)->GetCalcScore());
			// �˗��l���Z���N�g��Ԃ�
			auto itr = std::find(m_world->GetClients().begin(), m_world->GetClients().end(), (*mission)->GetStartActor());
			(*itr)->SetIsSelected(false);
			itr = std::find(m_world->GetClients().begin(), m_world->GetClients().end(), (*mission)->GetGoalActor());
			(*itr)->SetIsSelected(false);

			endMissions.emplace_back(*mission);
			mission = m_missions.erase(mission);
		}
		else
		{
			++mission;
		}

		listNum++;
	}
	// �폜����
	for (auto end : endMissions)
	{
		delete end;
	}
	endMissions.clear();


	// �~�b�V�����J�n�n�_�ƏI���n�_
    // �����Ő������A�J�n�E�I���_�Ŕ�肪�Ȃ������̃~�b�V�����Ɣ��Ȃ�����
	int startPos, goalPos;

	// �~�b�V�������X�g�͏펞3�B�����Ă�����쐬����
	int addMissionNum = MISSION_ALL_NUM - m_missions.size();
	if (addMissionNum > 0)
	{

		for (int i = 0; i < addMissionNum; i++)
		{
			InitRandom();
			// �~�b�V������ނ̗���
			int missionType = rand() % 2 + 1;
			// �˗��l�̍��W��V�K�̃~�b�V�����X�^�[�g���W�Ƃ��ăZ�b�g
			for (int j = 0; j < m_world->GetClients().size(); j++)
			{
				// �I������Ă��Ȃ�������X�^�[�g�n�_�Ƃ��Đݒ�
				if (!m_world->GetClients()[j]->GetIsSelected())
				{
					startPos = j;
					break;
				}
			}
			// �˗��l�̍��W��V�K�̃~�b�V�����S�[�����W�Ƃ��ăZ�b�g
			for (int j = m_world->GetClients().size() - 1; j >= 0; j--)
			{
				// �I������Ă��Ȃ�������X�^�[�g�n�_�Ƃ��Đݒ�
				if (!m_world->GetClients()[j]->GetIsSelected())
				{
					goalPos = j;
					break;
				}
			}

			// �~�b�V�����^�C�v���ƂɃ~�b�V�������e��ݒ�
			if (missionType == 1)
			{
				// �z�B�~�b�V�����̒ǉ�
				m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
				// �~�b�V�����T�v�Z�b�g
				m_missions.back()->SetMissionDetail(m_world->GetClients()[startPos],
					m_world->GetClients()[goalPos], 1000, 30);
			}
			else
			{
				// �쑗�~�b�V�����̒ǉ�
				m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
				// �~�b�V�����T�v�Z�b�g
				m_missions.back()->SetMissionDetail(m_world->GetClients()[startPos],
					m_world->GetClients()[goalPos], 1000, 30);
			}
		}
	}

	// �~�b�V�����J�[�\���̍X�V
	ChangeSelectNum();
}

// �����̏�����
void MissionManager::InitRandom()
{
	srand((unsigned int)time(NULL));
}

// ���Ԗڂ̃~�b�V������I�����Ă��邩��؂�ւ���
// �I�𒆂̃~�b�V�����̓����h�}�[�N�Ƃ���UI�ɕ\�������
void MissionManager::ChangeSelectNum()
{
	// �J�[�\�����ړ�
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_DOWN) || CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
	{
		m_selectNum++;
		// �~�b�V�������𒴂�����0�Ԗڂɐ܂�Ԃ�
		if (m_selectNum >= MISSION_ALL_NUM)
		{
			m_selectNum = 0;
		}
	}
	
	// �J�[�\����ړ�
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_DPAD_UP))
	{
		m_selectNum--;
		// 1��������܂�Ԃ�
		if (m_selectNum < 0)
		{
			m_selectNum = 2;
		}
	}

}
