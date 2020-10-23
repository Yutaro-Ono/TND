#include "PlayerManager.h"
#include "Input.h"
#include "InputController.h"
#include "ThirdPersonCamera.h"

PlayerManager::PlayerManager()
	:m_playerMode(PLAYER_MODE::MODE_HUMAN)
{
	m_playerHuman = new PlayerHuman(this);
	m_playerHuman->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
	m_playerHuman->SetScale(0.265f);

	m_playerCar = new PlayerCar();
	m_playerCar->SetPlayerManager(this);
	m_playerCar->SetPosition(Vector3(1700.0f, 2400.0f, 0.0f));
	m_playerCar->SetScale(0.3f);

	m_playerHuman->SetActive(true);
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::UpdateActor(float in_deltaTime)
{
	// 人間が車に近づいてYボタンを押すと乗る
	// 乗車中だったら降車
	if (Vector3::Distance(m_playerHuman->GetPosition(), m_playerCar->GetPosition()) <= 95.0f
		&& (CONTROLLER_INSTANCE.IsTriggered(SDL_CONTROLLER_BUTTON_Y) || INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_E)))
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

	// プレイヤー(人間)の座標を自身の座標とする
	m_position = m_playerHuman->GetPosition();
}
