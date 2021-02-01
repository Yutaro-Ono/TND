//---------------------------------------------------------+
//
// ゲームワールド統括クラス
//
//---------------------------------------------------------+
#pragma once
#include <vector>
#include "Environment.h"

class GameWorld
{

public:


	GameWorld();             // コンストラクタ
	~GameWorld();            // デストラクタ

	void Update(float in_deltaTime);

	// 配列への追加処理 (依頼人・巡回地点)
	void AddClientActor(class ClientActor* in_client);
	void AddPatrolPoint(class PatrolPoint* in_patrolPoint);

	class PlayerManager* GetPlayer() { return m_player; }

	// 巡回地点・依頼人アクタのシャッフル処理
	void ShufflePatrolPoint();
	void ShuffleClientActor();
	
	// 依頼人アクタの配列ゲッター
	const std::vector<class ClientActor*>& GetClients() { return m_clients; }

	// 巡回地点の配列ゲッター
	const std::vector<class PatrolPoint*>& GetPatrolPoint() { return m_patrolPoints; }

	// ヘリ配列のゲッター
	const std::vector<class Helicopter*>& GetHeliArray() { return m_helicopters; }

	void ImGuiDebugWorld();

	// 地形マネージャのポインタゲッター
	class LevelManager* GetLevelManager() { return m_level; }

	// UIキャンバスのゲッター
	class Canvas* GetCanvas() { return m_canvas; }

	bool GetFoundPlayer() { return m_foundPlayer; }

private:

	std::vector<class ClientActor*> m_clients;          // 依頼者アクター
	std::vector<class PatrolPoint*> m_patrolPoints;     // 巡回地点の配列
	std::vector<class Helicopter*> m_helicopters;        // ヘリコプターの配列

	class PlayerManager* m_player;

	class LevelManager* m_level;

	class MissionManager* m_mission;

	class Canvas* m_canvas;                     // UIキャンバス

	class MiniMapHUD* m_mapHUD;                 // マップHUD

	class Environment* m_environment;
	Environment::GAME_TIME m_gameTime;


	// フラグ管理
	bool m_foundPlayer;     // (一機でも)ヘリがプレイヤーを見つけたか
};