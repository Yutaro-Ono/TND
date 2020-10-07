#include "PlayerManager.h"
#include "Input.h"
#include "InputController.h"


PlayerManager::PlayerManager()
	:m_playerMode(PLAYER_MODE::MODE_HUMAN)
{
	m_playerHuman = new PlayerHuman(this);
	m_playerHuman->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));

	m_playerCar = new PlayerCar();
	m_playerCar->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::UpdateActor(float in_deltaTime)
{
	if (m_playerMode == PLAYER_MODE::MODE_HUMAN)
	{
		m_playerHuman->SetState(STATE_ACTIVE);
		m_playerCar->SetActive(false);
	}

	if (m_playerMode == PLAYER_MODE::MODE_CAR)
	{
		m_playerCar->SetActive(false);
	}
}
