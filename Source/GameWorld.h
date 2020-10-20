#pragma once
#include <vector>
//---------------------------------------------------------+
//
// �Q�[�����[���h�����N���X
//
//---------------------------------------------------------+

class GameWorld
{

public:

	GameWorld();             // �R���X�g���N�^
	~GameWorld();            // �f�X�g���N�^

	void Update(float in_deltaTime);

	void AddClientActor(class Actor* in_client);

	class PlayerManager* GetPlayer() { return m_player; }

	const std::vector<class Actor*>& GetClients() { return m_clients; }



private:

	std::vector<class Actor*> m_clients;          // �˗��҃A�N�^�[

	class PlayerManager* m_player;

	class LevelManager* m_level;

	class MissionManager* m_mission;

	class SkyBox* m_skyBox;

};