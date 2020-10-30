#include "MissionBase.h"
#include "ClientActor.h"
#include "MissionUI.h"
#include "MissionManager.h"
#include "PlayerManager.h"
#include "GameMain.h"
#include "GameConfig.h"

// �R���X�g���N�^
MissionBase::MissionBase(MissionManager* in_manager, MISSION_TYPE in_type, int in_listNum)
	:m_manager(in_manager)
	,m_startActor(nullptr)
	,m_goalActor(nullptr)
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

// �f�X�g���N�^
MissionBase::~MissionBase()
{
	m_missionUI->Close();
}

// �X�V����
void MissionBase::Update(float in_deltaTime)
{

	// �v���C���[���W���擾���Ă���
	Vector3 playerPos = m_manager->GetPlayer()->GetPosition();
	

	bool isSelected = false;
	// �~�b�V�������J�[�\���őI������Ă��鎞
	if (m_listNum == m_manager->GetSelectedMission())
	{
		isSelected = true;
	}
	else
	{
		// �I������Ă��Ȃ���Ε\�����Ȃ�
		m_startActor->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);
		m_goalActor->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);
	}

	//---------------------------------------------------+
	//
	// �X�e�[�g���Ƃ̍X�V����
	//
	//---------------------------------------------------+
	// �~�b�V�����ۗ���
	if (m_missionState == HOLD)
	{
		// UI��̃J�[�\���ł��̔C�����I������Ă���ꍇ�A�����h�}�[�N��\��
		if (isSelected)
		{
			m_startActor->SetClientSetting(ClientActor::CLIENT_SETTING::START);
		}


		// �ŏI�������X�V��������
		m_lastTime = SDL_GetTicks();

		// �v���C���[�̃A�N�Z�X�͈͋��Ɉ˗��l����������ԂŁA�{�^������������~�b�V�����J�n
		if (m_manager->GetPlayer()->GetSearchSphere().Contains(m_startActor->GetPosition()))
		{
			// �����{�^������������true
			if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_X) || INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_E))
			{
				m_startActor->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);
				// �C������������Ƃ��ăJ�E���g�_�E���J�n
				m_missionState = ACTIVE;
			}
		}

		//if (CheckDistPlayer(playerPos, m_startPos))
		//{
		//	m_startActor->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);
		//	// �C������������Ƃ��ăJ�E���g�_�E���J�n
		//	m_missionState = ACTIVE;
		//}


	}

	// �~�b�V�����󒍎�
	if (m_missionState == ACTIVE)
	{
		// UI��̃J�[�\���ł��̔C�����I������Ă���ꍇ�A�����h�}�[�N��\��
		if (isSelected)
		{
			m_goalActor->SetClientSetting(ClientActor::CLIENT_SETTING::GOAL);
		}

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
			m_goalActor->SetClientSetting(ClientActor::CLIENT_SETTING::NONE);
			m_missionState = SUCCESS;
		}

	}

}


// �~�b�V�����ڍאݒ�̒�`
void MissionBase::SetMissionDetail(ClientActor* in_start, ClientActor* in_goal, unsigned int in_baseScore, unsigned int in_timeLimit)
{
	m_startActor = in_start;
	m_goalActor = in_goal;

	m_startPos = m_startActor->GetPosition();          // �J�n�n�_
	m_goalPos = m_goalActor->GetPosition();            // �S�[���n�_

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
		if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_X) || INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_E))
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


// �X�R�A�𐬌������s���ɂ���Čv�Z���A�X�V�����X�R�A�l��Ԃ�
int MissionBase::GetCalcScore()
{
	if (m_missionState == MISSION_STATE::FAILED)
	{
		return 0;
	}

	return m_baseScore;
}

