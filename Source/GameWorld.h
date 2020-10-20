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

	void AddClientActor(class Actor* in_client);

	class PlayerManager* GetPlayer() { return m_player; }

	const std::vector<class Actor*>& GetClients() { return m_clients; }



private:

	std::vector<class Actor*> m_clients;          // 依頼者アクター

	class PlayerManager* m_player;

	class LevelManager* m_level;

	class MissionManager* m_mission;

	class SkyBox* m_skyBox;

};