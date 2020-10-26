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

	// �z��ւ̒ǉ����� (�˗��l�E����n�_)
	void AddClientActor(class ClientActor* in_client);
	void AddPatrolPoint(class PatrolPoint* in_patrolPoint);

	class PlayerManager* GetPlayer() { return m_player; }

	// �˗��l�A�N�^�̔z��Q�b�^�[
	const std::vector<class ClientActor*>& GetClients() { return m_clients; }

	// ����n�_�̔z��Q�b�^�[
	const std::vector<class PatrolPoint*>& GetPatrolPoint() { return m_patrolPoints; }
	void ShufflePatrolPoint();


	// �n�`�}�l�[�W���̃|�C���^�Q�b�^�[
	class LevelManager* GetLevelManager() { return m_level; }

private:

	std::vector<class ClientActor*> m_clients;          // �˗��҃A�N�^�[
	std::vector<class PatrolPoint*> m_patrolPoints;     // ����n�_�̔z��


	class PlayerManager* m_player;

	class Helicopter* m_helicopter[3];

	class LevelManager* m_level;

	class MissionManager* m_mission;

	class SkyBox* m_skyBox;

};