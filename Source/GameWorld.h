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

	void AddClientActor(class ClientActor* in_client);

	class PlayerManager* GetPlayer() { return m_player; }

	const std::vector<class ClientActor*>& GetClients() { return m_clients; }

	

private:

	std::vector<class ClientActor*> m_clients;          // �˗��҃A�N�^�[

	class PlayerManager* m_player;

	class Helicopter* m_helicopter;

	class LevelManager* m_level;

	class MissionManager* m_mission;

	class SkyBox* m_skyBox;

};