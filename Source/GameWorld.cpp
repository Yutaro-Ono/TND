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

// コンストラクタ
GameWorld::GameWorld()
{
	// プレイヤーの生成
	m_player = new PlayerManager();
	m_player->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
	m_player->SetScale(0.3f);

	// ヘリコプター生成
	m_helicopter = new Helicopter(Vector3(1800.0f, 2400.0f, 0.0f));

	// レベルの生成
	m_level = new LevelManager(this, 0);

	// ミッションマネージャー生成
	m_mission = new MissionManager(this);

	// スカイボックス生成
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

// 依頼人アクタの登録
void GameWorld::AddClientActor(ClientActor* in_client)
{
	m_clients.push_back(in_client);
}
