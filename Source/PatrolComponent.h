#pragma once
#include "Component.h"
#include "Math.h"

class PatrolComponent : public Component
{

public:
	
	// ������
	enum PATROL_STATE
	{
		NONE,       // �������Ȃ�
		PATROL,     // �ʏ폄��
		SEARCH,     // �v���C���[�̍��G
		CHASE       // �v���C���[�̒ǔ�
	};


	PatrolComponent(class Helicopter* in_owner, class GameWorld* in_world);
	~PatrolComponent();

	void Update(float in_deltaTime) override;      // �X�V����

	void MoveToPatrolPos(float in_deltaTime);

	void ChasePlayer(float in_deltaTime);          // �v���C���[�ǐ�

	void SetPatrolState(PatrolComponent::PATROL_STATE in_state) { m_patrolState = m_patrolState; }

private:


	PATROL_STATE m_patrolState;     // ������

	class Helicopter* m_heli;
	class GameWorld* m_world;
	
	class PatrolPoint* m_targetPoint;     // �����̃|�C���g
	Vector3 m_targetPos;                  // ����n�_�̍��W


};