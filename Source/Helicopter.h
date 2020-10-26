#pragma once
#include "Actor.h"
#include <string>
#include "Collision.h"

class Helicopter : public Actor
{

public:

	Helicopter(class GameWorld* in_world, const Vector3& in_pos);              // �R���X�g���N�^
	~Helicopter();             // �f�X�g���N�^

	void UpdateActor(float in_deltaTime) override;


	//---------------------------------------------------+
	// Getter / Setter
	//---------------------------------------------------+
	class GameWorld* GetWorld() { return m_world; }


private:

	// ���[���h�ւ̃|�C���^
	class GameWorld* m_world;

	// ����R���|�[�l���g
	class PatrolComponent* m_patrolComp;

	// �@�̂̕��i�N���X
	class HeliBody* m_body;                // �{�f�B
	class HeliRotorMain* m_mainRotor;      // ���C�����[�^�[
	class HeliRotorBack* m_backRotor;      // �o�b�N���[�^�[
	class HeliSpotLight* m_spotLight;      // ���C�g

	Sphere m_searchSphere;                 // ���G�͈͋�
};