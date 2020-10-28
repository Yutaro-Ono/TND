#include "PlayerManager.h"
#include "Input.h"
#include "InputController.h"
#include "ThirdPersonCamera.h"
#include "PhysicsWorld.h"

PlayerManager::PlayerManager()
	:m_playerMode(PLAYER_MODE::MODE_HUMAN)
	,m_searchSphere(Vector3::Zero, 0.0f)
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

void PlayerManager::UpdateActor(float in_deltaTime)
{
	// �ԂƐl�ԓ��m�̋��̓����蔻�� (�l�Ԋ)
	m_searchSphere.m_center = m_playerHuman->GetPosition();
	m_searchSphere.m_radius = 150.0f;

	// �l�Ԃ��Ԃɋ߂Â���Y�{�^���������Ə��
    // ��Ԓ���������~��
	if (m_searchSphere.Contains(m_playerCar->GetPosition()) && (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_Y) || INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_E)))
	{
		if (m_playerMode == MODE_HUMAN)
		{
			m_playerCar->OnChange();
			SetPlayerMode(MODE_CAR);
		}
		else if (m_playerMode == MODE_CAR)
		{
			m_playerHuman->GetCamera()->SetCameraForward(m_playerCar->GetCamera()->GetForward());
			SetPlayerMode(MODE_HUMAN);
		}
	}
	

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
