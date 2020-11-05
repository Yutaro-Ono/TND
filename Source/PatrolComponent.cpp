#include "PatrolComponent.h"
#include "GameWorld.h"
#include "PatrolPoint.h"
#include "Helicopter.h"
#include "PlayerManager.h"

// �R���X�g���N�^
PatrolComponent::PatrolComponent(Helicopter* in_owner, GameWorld* in_world)
	:Component(in_owner)
	,m_patrolState(PATROL_STATE::PATROL)
	,m_heli(in_owner)
	,m_world(in_world)
	,m_targetPoint(nullptr)
{
	// ����n�_�𑖍����ď����̍��W�Ƃ��ăZ�b�g
	for (auto p : m_world->GetPatrolPoint())
	{
		// ���łɏ���n�_�Ƃ��ēo�^����Ă��Ȃ�������
		if (!p->GetIsSelected())
		{
			// �����Ƃ��ĕۑ�
			m_targetPoint = p;
			break;
		}
	}

	// ����n�_�Ƃ��đI�����ꂽ���Ƃ�`����
	m_targetPoint->SetIsSelected(true);
}

// �f�X�g���N�^
PatrolComponent::~PatrolComponent()
{
}

// �X�V����
void PatrolComponent::Update(float in_deltaTime)
{

	// �w�����v���C���[����������
	if (m_heli->GetFoundPlayer())
	{
		m_patrolState = PATROL_STATE::CHASE;
	}

	// �����Ԏ�����
	if (m_patrolState == PATROL_STATE::PATROL)
	{
		// �^�[�Q�b�g���W���X�V
		m_targetPos = m_targetPoint->GetPosition();

		MoveToPatrolPos(in_deltaTime);
	}

	// �v���C���[�ǐՏ���
	if (m_patrolState == PATROL_STATE::CHASE)
	{
		m_targetPos = m_world->GetPlayer()->GetPosition() + Vector3(0.0f, 300.0f * m_heli->GetNumber(), 600.0f + 100.0f * m_heli->GetNumber()) + m_world->GetPlayer()->GetForward() * Vector3(-300.0f, 0.0f, 0.0f);
		ChasePlayer(in_deltaTime);
	}
}

// ����n�_�ւ̈ړ�����
void PatrolComponent::MoveToPatrolPos(float in_deltaTime)
{
	// ����n�_�ύX���ɗp����|�C���^
	PatrolPoint* tempPoint = m_targetPoint;

	// �w���R�v�^�[�̌��ݒn�_���W
	Vector3 heliPos = m_owner->GetPosition();

	// (�^�[�Q�b�g���W - �w�����W)����O�i�x�N�g��������o��
	Vector3 forwardVec = m_targetPos - heliPos;
	forwardVec.Normalize();
	// �w��������n�_�֌�������
	m_owner->RotateToNewForward(forwardVec);

	// �ړ��ʂ��v�Z
	Vector3 move = forwardVec * 800.0f * in_deltaTime;

	// �w�����ړ�������
	m_owner->SetPosition(heliPos + move);
	//m_owner->SetPosition(Vector3::Lerp(playerPos, m_targetPos, 0.03f * in_deltaTime));


	// �ڕW�n�_�֐ڋ߂����珄��n�_���X�V
	if (Vector3::Distance(heliPos, m_targetPos) < 100.0f)
	{
		// ����n�_�𑖍����ď����̍��W�Ƃ��ăZ�b�g
		for (auto p : m_world->GetPatrolPoint())
		{
			// ����n�_�Ƃ��ēo�^����Ă��Ȃ�������
			if (!p->GetIsSelected())
			{
				// �����Ƃ��ĕۑ�
				tempPoint = p;
				break;
			}
		}
	}



	// ����n�_���X�V���ꂽ��
	if (m_targetPoint != tempPoint)
	{
		// ����n�_�������_���\�[�g
		m_world->ShufflePatrolPoint();

		tempPoint->SetIsSelected(true);
		// ����悾�����n�_�̑I����ԉ���
		m_targetPoint->SetIsSelected(false);

		// ���̏���n�_�Ƃ��čX�V
		m_targetPoint = tempPoint;
	}
}

void PatrolComponent::ChasePlayer(float in_deltaTime)
{

	// �w���R�v�^�[�̌��ݒn�_���W
	Vector3 heliPos = m_owner->GetPosition();
	// (�^�[�Q�b�g���W - �w�����W)����O�i�x�N�g��������o��
	Vector3 forwardVec = m_targetPos - heliPos;
	forwardVec.Normalize();

	float distance = Vector3::Distance(heliPos, m_targetPos);
	// �v���C���[�����苗�����ꂽ��|�C���g����ɖ߂�
	if (distance > 5001.0f)
	{
		m_patrolState = PATROL_STATE::PATROL;
		return;
	}
	
	if (distance > 100.0f)
	{
		// �ړ��ʂ��v�Z
		Vector3 move = forwardVec * 300.0f * in_deltaTime;
		// �w�����ړ�������
		m_owner->SetPosition(heliPos + move);

		// �w��������n�_�֌�������
		forwardVec.z = 0.0f;
		m_owner->RotateToNewForward(forwardVec);

	}
	else
	{
		
	}

}
