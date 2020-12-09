#pragma once
#include "Actor.h"
#include <string>
#include "Collision.h"

class Helicopter : public Actor
{

public:

	enum HELI_STATE
	{
		PATROL = 0,
		CHASE,
		STOP
	};

	Helicopter(class GameWorld* in_world, const Vector3& in_pos, int in_num);              // �R���X�g���N�^
	~Helicopter();                                                             // �f�X�g���N�^

	void UpdateActor(float in_deltaTime) override;

	void SearchPlayer(class PlayerManager* in_player);

	//---------------------------------------------------+
	// Getter / Setter
	//---------------------------------------------------+
	class GameWorld* GetWorld()  { return m_world; }
	bool  GetFoundPlayer()       { return m_foundPlayer; }

	// �X�e�[�g�̃Q�b�^�[�E�Z�b�^�[
	HELI_STATE GetHeliState() { return m_state; }
	void SetHeliState(HELI_STATE in_state) { m_state = in_state; }

	int GetNumber() { return m_number; }

private:


	// �w���̃X�e�[�g
	HELI_STATE m_state;

	// ���[���h�ւ̃|�C���^
	class GameWorld* m_world;

	// ����R���|�[�l���g
	class PatrolComponent* m_patrolComp;

	// �@�̂̕��i�N���X
	class HeliBody* m_body;                // �{�f�B
	class HeliRotorMain* m_mainRotor;      // ���C�����[�^�[
	class HeliRotorBack* m_backRotor;      // �o�b�N���[�^�[
	class HeliSpotLight* m_spotLight;      // ���C�g

	class PointLight* m_pLight[2];         // �|�C���g���C�g

	int m_number;                          // ���[���h��ł̃w���̔ԍ�

	bool m_foundPlayer;                    // �v���C���[�����������ǂ���
	Sphere m_searchSphere;                 // ���G�͈͋�
};