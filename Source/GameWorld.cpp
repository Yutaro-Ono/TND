#include "GameWorld.h"
#include "GameScene.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "AudioManager.h"
#include "Renderer.h"
#include "ParticleManager.h"
#include "LevelManager.h"
#include "SkyBox.h"
#include "Math.h"
#include "PlayerCar.h"
#include "PlayerManager.h"
#include "MissionManager.h"
#include "Helicopter.h"

#include <Windows.h>
#include <iostream>

// �R���X�g���N�^
GameWorld::GameWorld()
{
	// �v���C���[�̐���
	m_player = new PlayerManager();
	m_player->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
	m_player->SetScale(0.3f);

	// �w���R�v�^�[����
	m_helicopter = new Helicopter(Vector3(1800.0f, 2400.0f, 0.0f));

	// ���x���̐���
	m_level = new LevelManager(this, 0);

	// �~�b�V�����}�l�[�W���[����
	m_mission = new MissionManager(this);

	// �X�J�C�{�b�N�X����
	m_skyBox = new SkyBox();

}

GameWorld::~GameWorld()
{
	m_clients.clear();
	delete m_player;
	delete m_helicopter;
	delete m_level;
	delete m_mission;
	delete m_skyBox;
}

void GameWorld::Update(float in_deltaTime)
{
	m_mission->Update(in_deltaTime);
}

// �˗��l�A�N�^�̓o�^
void GameWorld::AddClientActor(ClientActor* in_client)
{
	m_clients.push_back(in_client);
}
