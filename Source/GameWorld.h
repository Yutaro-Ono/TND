#pragma once
#include <vector>
//---------------------------------------------------------+
//
// ゲームワールド統括クラス
//
//---------------------------------------------------------+

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

	// 依頼人アクタの配列ゲッター
	const std::vector<class ClientActor*>& GetClients() { return m_clients; }

	// 巡回地点の配列ゲッター
	const std::vector<class PatrolPoint*>& GetPatrolPoint() { return m_patrolPoints; }
	void ShufflePatrolPoint();


	// 地形マネージャのポインタゲッター
	class LevelManager* GetLevelManager() { return m_level; }

private:

	std::vector<class ClientActor*> m_clients;          // 依頼者アクター
	std::vector<class PatrolPoint*> m_patrolPoints;     // 巡回地点の配列


	class PlayerManager* m_player;

	class Helicopter* m_helicopter[3];

	class LevelManager* m_level;

	class MissionManager* m_mission;

	class SkyBox* m_skyBox;

};