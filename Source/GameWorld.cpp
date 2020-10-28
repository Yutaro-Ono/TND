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

// コンストラクタ
GameWorld::GameWorld()
{

	// プレイヤーの生成
	m_player = new PlayerManager();
	m_player->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
	m_player->SetScale(0.3f);


	// レベルの生成
	m_level = new LevelManager(this, 0);

	// ミッションマネージャー生成
	m_mission = new MissionManager(this);

	// 環境(光源など)作成
	//m_environment = new Environment(Environment::GAME_TIME::NIGHT);
	m_environment = new Environment(Environment::GAME_TIME::MORNING);


	// ヘリコプター生成
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

// 依頼人アクタの登録
void GameWorld::AddClientActor(ClientActor* in_client)
{
	m_clients.push_back(in_client);
}

// 巡回地点の登録
void GameWorld::AddPatrolPoint(PatrolPoint* in_patrolPoint)
{
	m_patrolPoints.push_back(in_patrolPoint);
}

// 巡回地点のシャッフル
void GameWorld::ShufflePatrolPoint()
{
	std::random_shuffle(m_patrolPoints.begin(), m_patrolPoints.end());
}

// 依頼人アクタ配列のシャッフル
void GameWorld::ShuffleClientActor()
{
	std::random_shuffle(m_clients.begin(), m_clients.end());
}

void GameWorld::ImGuiDebugWorld()
{
}
