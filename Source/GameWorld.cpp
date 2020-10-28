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
#include <algorithm>

// �R���X�g���N�^
GameWorld::GameWorld()
{

	// �v���C���[�̐���
	m_player = new PlayerManager();
	m_player->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
	m_player->SetScale(0.3f);


	// ���x���̐���
	m_level = new LevelManager(this, 0);

	// �~�b�V�����}�l�[�W���[����
	m_mission = new MissionManager(this);

	// ��(�����Ȃ�)�쐬
	//m_environment = new Environment(Environment::GAME_TIME::NIGHT);
	m_environment = new Environment(Environment::GAME_TIME::MORNING);


	// �w���R�v�^�[����
	for (int i = 0; i < 3; i++)
	{
		m_helicopter[i] = new Helicopter(this, Vector3(600.0f * i, 800.0f * i, 1000.0f));
	}


}

GameWorld::~GameWorld()
{
	m_clients.clear();
	m_patrolPoints.clear();
	delete m_player;
	delete m_level;
	delete m_mission;
	delete m_environment;
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

// ����n�_�̓o�^
void GameWorld::AddPatrolPoint(PatrolPoint* in_patrolPoint)
{
	m_patrolPoints.push_back(in_patrolPoint);
}

// ����n�_�̃V���b�t��
void GameWorld::ShufflePatrolPoint()
{
	std::random_shuffle(m_patrolPoints.begin(), m_patrolPoints.end());
}

// �˗��l�A�N�^�z��̃V���b�t��
void GameWorld::ShuffleClientActor()
{
	std::random_shuffle(m_clients.begin(), m_clients.end());
}

void GameWorld::ImGuiDebugWorld()
{
}
