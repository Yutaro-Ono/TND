#include "MissionManager.h"
#include "GameWorld.h"
#include "Canvas.h"
#include "MissionBase.h"
#include "MissionUI.h"
#include "ClientActor.h"
#include "Input.h"
#include "InputController.h"
#include "ScoreUI.h"
#include "PlayerManager.h"
#include "AudioManager.h"
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

	// �~�b�V�����J�n�n�_�ƏI���n�_
	// �����Ő������A�J�n�E�I���_�Ŕ�肪�Ȃ������̃~�b�V�����Ɣ��Ȃ�����
	int startPos[MISSION_ALL_NUM], goalPos[MISSION_ALL_NUM];
	
	// �˗��l�A�N�^�z����V���b�t��
	m_world->ShuffleClientActor();

	// �~�b�V�������X�g�̐���
	for (int i = 0; i < MISSION_ALL_NUM; i++)
	{
		// �~�b�V������ނ̗���
		int missionType = rand() % 2 + 1;
		
		startPos[i] = -1;
		// �˗��l�A�N�^�̒�����I����ԂłȂ��˗��l��T���A�X�^�[�g�n�_�A�N�^�̔ԍ����擾����
		for (int j = 0; j < m_world->GetClients().size(); j++)
		{

			if (m_world->GetClients()[j]->GetClientSetting() == ClientActor::NONE)
			{
				startPos[i] = j;

				m_world->GetClients()[j]->SetClientSetting(ClientActor::START);
			}
			// �X�^�[�g�n�_���ύX����Ă����烋�[�v�𔲂���
			if (startPos[i] >= 0)
			{
				break;
			}
		}

		goalPos[i] = -1;
		// �˗��l�A�N�^�̒�����I����ԂłȂ��˗��l��T���A�S�[���n�_�A�N�^�̔ԍ����擾����
		for (int j = 0; j < m_world->GetClients().size(); j++)
		{

			if (m_world->GetClients()[j]->GetClientSetting() == ClientActor::NONE)
			{
				goalPos[i] = j;

				m_world->GetClients()[j]->SetClientSetting(ClientActor::GOAL);
			}
			// �S�[���n�_���ύX����Ă����烋�[�v�𔲂���
			if (goalPos[i] >= 0)
			{
				break;
			}
		}

		// �v���C���[�̌��ݒn�E�X�^�[�g�n�_�ƃS�[���n�_�̋�������X�R�A���Z�o
		float score = CalcScoreForDistance(m_player->GetPosition(),
			                               m_world->GetClients()[startPos[i]]->GetPosition(), 
			                               m_world->GetClients()[goalPos[i]]->GetPosition());

		// �~�b�V�����^�C�v���ƂɃ~�b�V�������e��ݒ�
		if (missionType == 1)
		{
			// �z�B�~�b�V�����̒ǉ�
			m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
			// �~�b�V�����T�v�Z�b�g
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos[i]],
				m_world->GetClients()[goalPos[i]], score, 60);
		}
		else
		{
			// �쑗�~�b�V�����̒ǉ�
			m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
			// �~�b�V�����T�v�Z�b�g
			m_missions[i]->SetMissionDetail(m_world->GetClients()[startPos[i]],
				m_world->GetClients()[goalPos[i]], score, 50);
		}

		// �˗��l�A�N�^�z����V���b�t��
		m_world->ShuffleClientActor();
	}


	// ���ʉ��̃��[�h
	m_selectSE = "Data/Music/SE/TND/System/Select/decide14.wav";
	AUDIO->GetSound(m_selectSE);

}

// �f�X�g���N�^
MissionManager::~MissionManager()
{
	m_missions.clear();
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
			m_world->GetCanvas()->GetScoreUI()->SetAddScore((*mission)->GetCalcScore());
			// �˗��l���Z���N�g��Ԃ�
			auto itr = std::find(m_world->GetClients().begin(), m_world->GetClients().end(), (*mission)->GetStartActor());
			(*itr)->SetIsSelected(false);
			(*itr)->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);
			itr = std::find(m_world->GetClients().begin(), m_world->GetClients().end(), (*mission)->GetGoalActor());
			(*itr)->SetIsSelected(false);
			(*itr)->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);

			// �~�b�V�����V���b�t��
			m_world->ShuffleClientActor();

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
	startPos = goalPos = 0;
	// �~�b�V�������X�g�͏펞3�B�����Ă�����쐬����
	int addMissionNum = MISSION_ALL_NUM - m_missions.size();
	if (addMissionNum > 0)
	{

		for (int i = 0; i < addMissionNum; i++)
		{
			InitRandom();
			// �~�b�V������ނ̗���
			int missionType = rand() % 2 + 1;
			// �˗��l�̍��W��V�K�̃~�b�V�����S�[�����W�Ƃ��ăZ�b�g
			for (int j = m_world->GetClients().size() - 1; j >= 0; j--)
			{
				// �I������Ă��Ȃ�������X�^�[�g�n�_�Ƃ��Đݒ�
				if (m_world->GetClients()[j]->GetClientSetting() == ClientActor::NONE)
				{
					startPos = j;
					break;
				}
			}

			// �˗��l�̍��W��V�K�̃~�b�V�����X�^�[�g���W�Ƃ��ăZ�b�g
			for (int j = 0; j < m_world->GetClients().size(); j++)
			{
				// �I������Ă��Ȃ�������X�^�[�g�n�_�Ƃ��Đݒ�
				if (m_world->GetClients()[j]->GetClientSetting() == ClientActor::NONE)
				{
					goalPos = j;
					break;
				}
			}

			// �v���C���[�̌��ݒn�E�X�^�[�g�n�_�ƃS�[���n�_�̋�������X�R�A���Z�o
			float score = CalcScoreForDistance(m_player->GetPosition(), m_world->GetClients()[startPos]->GetPosition(),
				                               m_world->GetClients()[goalPos]->GetPosition());

			// �~�b�V�����^�C�v���ƂɃ~�b�V�������e��ݒ�
			if (missionType == 1)
			{
				// �z�B�~�b�V�����̒ǉ�
				m_missions.push_back(new MissionBase(this, MissionBase::DELIVERY, i));
				// �~�b�V�����T�v�Z�b�g
				m_missions.back()->SetMissionDetail(m_world->GetClients()[startPos],
					m_world->GetClients()[goalPos], score, 60);
				// �N���C�A���g���Z���N�g��Ԃ�
				m_world->GetClients()[startPos]->SetClientSetting(ClientActor::START);
				m_world->GetClients()[goalPos]->SetClientSetting(ClientActor::GOAL);

			}
			else
			{
				// �쑗�~�b�V�����̒ǉ�
				m_missions.push_back(new MissionBase(this, MissionBase::TAXI, i));
				// �~�b�V�����T�v�Z�b�g
				m_missions.back()->SetMissionDetail(m_world->GetClients()[startPos],
					m_world->GetClients()[goalPos], score, 50);
				// �N���C�A���g���Z���N�g��Ԃ�
				m_world->GetClients()[startPos]->SetClientSetting(ClientActor::START);
				m_world->GetClients()[goalPos]->SetClientSetting(ClientActor::GOAL);

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

		// �I��ԍ��v���X
		m_selectNum++;

		// ���ʉ��Đ�
		AUDIO->PlaySoundTND(m_selectSE);

		// �~�b�V�������𒴂�����0�Ԗڂɐ܂�Ԃ�
		if (m_selectNum >= MISSION_ALL_NUM)
		{
			m_selectNum = 0;
		}
	}
	
	// �J�[�\����ړ�
	if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_UP) || CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_DPAD_UP))
	{

		// �I��ԍ��}�C�i�X
		m_selectNum--;

		// ���ʉ��Đ�
		AUDIO->PlaySoundTND(m_selectSE);

		// 1��������܂�Ԃ�
		if (m_selectNum < 0)
		{
			m_selectNum = 2;
		}
	}

}

// �����ɉ������X�R�A�v�Z
float MissionManager::CalcScoreForDistance(const Vector3& in_pPos, const Vector3& in_cPos1, const Vector3& in_cPos2)
{
	// �x�[�X�X�R�A�|�C���g
	float baseScore = 100.0f;
	float score = 0.0f;

	// 1.�v���C���[�ƃX�^�[�g�n�_�N���C�A���g�̋��� + �X�^�[�g�n�_�ƃS�[���n�_�̋����̍��v����{�[�i�X�X�R�A���Z�o
	score = (Vector3::Distance(in_pPos, in_cPos1) / 20) + (Vector3::Distance(in_cPos1, in_cPos2) / 20);

	return baseScore + score;
}
