#include "MissionBase.h"
#include "Actor.h"
#include "MissionUI.h"
#include "MissionManager.h"
#include "PlayerManager.h"
#include "GameMain.h"
#include "GameConfig.h"

MissionBase::MissionBase(MissionManager* in_manager, MISSION_TYPE in_type, int in_listNum)
	:m_manager(in_manager)
	,m_missionType(in_type)
	,m_missionState(HOLD)
	,m_durableVal(100)
	, m_listNum(in_listNum)
{
	// ������
	m_startPos = m_goalPos = Vector3::Zero;
	m_currentTime = m_lastTime = SDL_GetTicks();

	// UI����
	m_missionUI = new MissionUI(this);
	m_missionUI->SetUIPosition();
}

MissionBase::~MissionBase()
{
	m_missionUI->Close();
}

// �X�V����
void MissionBase::Update(float in_deltaTime)
{

	// �v���C���[���W���擾���Ă���
	Vector3 playerPos = m_manager->GetPlayer()->GetPosition();
	

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

		// �S�[���n�_�Ƀv���C���[���ڐG���A�{�^������������~�b�V�����J�n
		if (CheckDistPlayer(playerPos, m_startPos))
		{
			m_missionState = ACTIVE;
		}
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
			// ���s��Ԃɂ���
			m_missionState = FAILED;
		}

		// �ϋv�l��0�ɂȂ�����~�b�V�������s
		if (m_durableVal <= 0)
		{
			m_missionState = FAILED;
		}

		// �S�[���n�_�Ƀv���C���[���ڐG���A�{�^������������~�b�V�����I��(����)
		if (CheckDistPlayer(playerPos, m_goalPos))
		{
			m_missionState = SUCCESS;
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

// �v���C���[�̍��W�`�J�n�n�_or�I���n�_���W�̋��������߁A���ȏ�ڋ߂��ă{�^������������^)
bool MissionBase::CheckDistPlayer(const Vector3& in_playerPos, const Vector3& in_missionPos)
{
	// �v���C���[�Ƃ̋����𑪒肷��
	m_playerDistance = Vector3::Distance(in_playerPos, in_missionPos);
	// ���������ȏ�߂��Ƃ�
	if (m_playerDistance < 95.0f)
	{
		// �����{�^������������true
		if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_A) || INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_E))
		{
			return true;
		}
	}

	return false;
}

// �ϋv�l��������
void MissionBase::DecraseDurableValue()
{
	m_durableVal -= 3;
}

