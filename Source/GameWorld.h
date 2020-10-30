//---------------------------------------------------------+
//
// �Q�[�����[���h�����N���X
//
//---------------------------------------------------------+
#pragma once
#include <vector>
#include "Environment.h"

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

	// ����n�_�E�˗��l�A�N�^�̃V���b�t������
	void ShufflePatrolPoint();
	void ShuffleClientActor();
	
	// �˗��l�A�N�^�̔z��Q�b�^�[
	const std::vector<class ClientActor*>& GetClients() { return m_clients; }

	// ����n�_�̔z��Q�b�^�[
	const std::vector<class PatrolPoint*>& GetPatrolPoint() { return m_patrolPoints; }

	// �w���z��̃Q�b�^�[
	const std::vector<class Helicopter*>& GetHeliArray() { return m_helicopters; }

	void ImGuiDebugWorld();


	// �n�`�}�l�[�W���̃|�C���^�Q�b�^�[
	class LevelManager* GetLevelManager() { return m_level; }

private:

	std::vector<class ClientActor*> m_clients;          // �˗��҃A�N�^�[
	std::vector<class PatrolPoint*> m_patrolPoints;     // ����n�_�̔z��
	std::vector<class Helicopter*> m_helicopters;        // �w���R�v�^�[�̔z��

	class PlayerManager* m_player;

	class LevelManager* m_level;

	class MissionManager* m_mission;


	class Environment* m_environment;
	Environment::GAME_TIME m_gameTime;

};