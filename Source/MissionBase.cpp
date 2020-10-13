#include "MissionBase.h"
#include "Actor.h"
#include "MissionUI.h"

MissionBase::MissionBase(Actor* in_client)
	:m_client(in_client)
	,m_missionState(HOLD)
{
	// ������
	m_startPos = m_goalPos = Vector3::Zero;
	m_currentTime = m_lastTime = SDL_GetTicks();

	// UI�̐���
	m_missionUI = new MissionUI(this);
}

MissionBase::~MissionBase()
{
}

// �X�V����
void MissionBase::Update(float in_deltaTime)
{

	//---------------------------------------------------+
	//
	// �X�e�[�g���Ƃ̍X�V����
	//
	//---------------------------------------------------+
	// �~�b�V�����ۗ���
	if (m_missionState == HOLD)
	{
		// �ŏI�������X�V��������
		m_lastTime = SDL_GetTicks();
	}

	// �~�b�V�����󒍎�
	if (m_missionState == ACTIVE)
	{

		// ���ݎ������X�V
		m_currentTime = SDL_GetTicks();

		//-----------------------------------------+
		// �J�E���g�_�E������
		//-----------------------------------------+
		// ��b�o�ߌ�
		if (m_currentTime > m_lastTime + 1000)
		{
			// �������Ԃ�1���炷
			m_timeLimit -= 1;
			// �ŏI�����X�V
			m_lastTime = m_currentTime;
		}

		// �������Ԃ��؂ꂽ��~�b�V�������s
		if (m_timeLimit <= 0)
		{
			// UI�𗎂Ƃ�
			m_missionUI->Close();
			// ���s��Ԃɂ���
			m_missionState = FAILED;
		}
	}




}


// �~�b�V�����ڍאݒ�̒�`
void MissionBase::SetMissionDetail(const Vector3& in_start, const Vector3& in_goal, unsigned int in_baseScore, unsigned int in_timeLimit)
{
	m_startPos = in_start;          // �J�n�n�_
	m_goalPos = in_goal;            // �S�[���n�_

	m_baseScore = in_baseScore;     // �x�[�X�X�R�A
	m_timeLimit = in_timeLimit;     // ��������
}

