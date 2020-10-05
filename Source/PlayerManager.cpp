#include "PlayerManager.h"
#include "PlayerHuman.h"
#include "PlayerCar.h"
#include "Input.h"
#include "InputController.h"


PlayerManager::PlayerManager()
	:m_playerMode(PLAYER_MODE::MODE_HUMAN)
{
	
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
