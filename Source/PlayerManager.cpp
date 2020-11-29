#include "PlayerManager.h"
#include "Input.h"
#include "InputController.h"
#include "ThirdPersonCamera.h"
#include "ThirdPersonCarCamera.h"
#include "PhysicsWorld.h"
#include "ClientActor.h"

PlayerManager::PlayerManager()
	:m_playerMode(PLAYER_MODE::MODE_HUMAN)
	,m_searchSphere(Vector3::Zero, 150.0f)
	,m_isDetectedCar(false)
	,m_isDetectedClient(false)
{
	m_playerHuman = new PlayerHuman(this);
	m_playerHuman->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
	m_playerHuman->SetScale(0.265f);

	m_playerCar = new PlayerCar();
	m_playerCar->SetPlayerManager(this);
	m_playerCar->SetPosition(Vector3(1700.0f, 2400.0f, -15.0f));
	m_playerCar->SetScale(0.3f);

	m_playerHuman->SetActive(true);
}

PlayerManager::~PlayerManager()
{
}

// �X�V����
void PlayerManager::UpdateActor(float in_deltaTime)
{

	// �l�ԃv���C���[�̓����蔻��X�V
	m_searchSphere.m_center = m_playerHuman->GetPosition();

    // �v���C���[�ƎԂ̏�ԁE�~�ԏ���
	SearchCar();

	// ���t���[���˗��l�����t���O�̓I�t�ɂ���(true���̏㏑����h������)
	m_isDetectedClient = false;

	if (m_playerMode == PLAYER_MODE::MODE_HUMAN)
	{
		m_playerHuman->SetState(STATE_ACTIVE);
		m_playerCar->SetActive(false);
	}

	if (m_playerMode == PLAYER_MODE::MODE_CAR)
	{
		m_playerCar->SetActive(true);
		m_playerHuman->SetPosition(m_playerCar->GetPosition());
	}

	// �v���C���[(�l��)�̍��W�����g�̍��W�Ƃ���
	m_position = m_playerHuman->GetPosition();
}



// �v���C���[�̃A�N�Z�X�͈͓��ɎԂ����邩�̌���
// �Ԃ̏�ԁE�~�ԏ���
void PlayerManager::SearchCar()
{

	// �v���C���[�̃A�N�Z�X�͈͂ɎԂ������Ă�����t���O�𗧂Ă�
	if (m_searchSphere.Contains(m_playerCar->GetPosition()) && m_playerMode == MODE_HUMAN)
	{
		m_isDetectedCar = true;
	}
	else
	{
		m_isDetectedCar = false;
	}

	// �A�N�Z�X�͈͓��ɎԂ������Ă�����
	// �l�Ԃ��Ԃɋ߂Â���Y�{�^���������Ə��
	// ��Ԓ���������~��
	if (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_Y) || INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_E))
	{
		if (m_isDetectedCar)
		{
			m_playerCar->OnChange();
			SetPlayerMode(MODE_CAR);
		}
		// �Ԃ���~�������͈��ȉ��̑��x�ł��邱��
		else if (m_playerMode == MODE_CAR && m_playerCar->GetAccelValue() <= 35.0f)
		{
			m_playerHuman->GetCamera()->SetCameraForward(m_playerCar->GetCamera()->GetForward());
			m_playerHuman->SetPosition(m_playerHuman->GetPosition() + Vector3(40.0f, 40.0f, 0.0f));
			SetPlayerMode(MODE_HUMAN);
		}
	}

}

// �v���C���[�̃A�N�Z�X�͈͓��Ɉ˗��l�����邩������
void PlayerManager::SearchClient(ClientActor* in_client)
{
	// ���������~�b�V�����Ώۂ̈˗��l���E�˗��l�����łɔ������Ă����疳��
	if (in_client->GetClientSetting() == ClientActor::NONE || m_isDetectedClient == true)
	{
		return;
	}

	in_client->SetDistancePlayer(Vector3::Distance(m_position, in_client->GetPosition()));   // �������Z�b�g

	// �n���ꂽ�˗��l�̍��W���v���C���[�̃A�N�Z�X�͈͓��ɓ����Ă��邩
	if (m_searchSphere.Contains(in_client->GetPosition()))
	{
		in_client->SetAcceptedPlayer();
		m_isDetectedClient = in_client->GetIsAccepted();
	}
	else
	{
		in_client->SetIsAccepted(false);
	}
	
}
