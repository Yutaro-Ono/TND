#include "GameWorld.h"
#include "GameScene.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "Renderer.h"
#include "LevelManager.h"
#include "SkyBox.h"
#include "Math.h"
#include "PlayerCar.h"
#include "PlayerManager.h"
#include "MissionManager.h"
#include "Helicopter.h"
#include "Canvas.h"
#include "PlayerControlUI.h"
#include "PointLight.h"
#include "LandMarkIndicator.h"
#include "MiniMapHUD.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>

static Vector3 playerPos;
static Vector3 tempPos;

// コンストラクタ
GameWorld::GameWorld()
	:m_foundPlayer(false)
{
	// プレイヤーの生成
	m_player = new PlayerManager();
	m_player->SetPosition(Vector3(1800.0f, 2400.0f, 0.0f));
	m_player->SetScale(200.0f);
	//PointLight* light = new PointLight();
	//light->SetPosition(m_player->GetPosition());
	tempPos = playerPos = m_player->GetPosition();

	// レベルの生成
	m_level = new LevelManager(this, 0);
	// ミッションマネージャー生成
	m_mission = new MissionManager(this);
	// 環境(光源など)作成
	m_environment = new Environment(this, Environment::GAME_TIME::NIGHT);
	//m_environment = new Environment(this, Environment::GAME_TIME::MORNING);

	// ヘリコプターを三機生成
	for (int i = 0; i < 3; i++)
	{
		m_helicopters.emplace_back(new Helicopter(this, Vector3(5000.0f * (i + 1), 18000.0f * (i + 1), 1200.0f), i));
		//m_helicopters.emplace_back(new Helicopter(this, Vector3(1800.0f, 2400.0f, 1200.0f), i));
	}

	// UI生成
	m_canvas = new Canvas(this);

	// マップHUD生成
	m_mapHUD = new MiniMapHUD(m_player);
}

GameWorld::~GameWorld()
{
	RENDERER->RemoveMapHUD();

	for (auto heli : m_helicopters)
	{
		heli->SetState(Actor::STATE_DEAD);
	}
	m_helicopters.clear();
	m_clients.clear();
	m_patrolPoints.clear();
	delete m_player;
	delete m_canvas;
	delete m_level;
	delete m_mission;
	delete m_environment;
}

void GameWorld::Update(float in_deltaTime)
{

	m_environment->Update();

	// ミッションの更新
	m_mission->Update(in_deltaTime);

	// プレイヤーの依頼人サーチ
	for (auto client : m_clients)
	{
		m_player->SearchClient(client);
	}

	// ヘリがプレイヤーを発見したかどうかを走査し、一つでもヒットしたら抜ける
	for (auto heli : m_helicopters)
	{
		if (heli->GetFoundPlayer())
		{
			m_canvas->GetPlayerControlUI()->SetFindPlayer(true);
			m_foundPlayer = true;
			break;
		}
		else
		{
			m_canvas->GetPlayerControlUI()->SetFindPlayer(false);
			m_foundPlayer = false;
		}
	}
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
	std::random_shuffle(m_clients.begin(), m_clients.end() - 1);
}

void GameWorld::ImGuiDebugWorld()
{
}
