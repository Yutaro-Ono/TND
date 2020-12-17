#include "PatrolComponent.h"
#include "GameWorld.h"
#include "PatrolPoint.h"
#include "Helicopter.h"
#include "PlayerManager.h"
#include <Windows.h>

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

	// �e�w���������ԂɃZ�b�g
	m_heli->SetHeliState(Helicopter::HELI_STATE::PATROL);
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
		m_heli->SetHeliState(Helicopter::HELI_STATE::CHASE);
	}

	// �����Ԏ�����
	if (m_heli->GetHeliState() == Helicopter::HELI_STATE::PATROL)
	{
		// �^�[�Q�b�g���W���X�V
		m_targetPos = m_targetPoint->GetPosition();

		MoveToPatrolPos(in_deltaTime);
	}

	// �v���C���[�ǐՏ���
	if (m_heli->GetHeliState() == Helicopter::HELI_STATE::CHASE)
	{
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

// �v���C���[�̒ǔ�
void PatrolComponent::ChasePlayer(float in_deltaTime)
{

	// �w���R�v�^�[�̌��ݒn�_���W
	Vector3 heliPos = m_owner->GetPosition();
	// �v���C���[�̍��W
	Vector3 playerPos = m_world->GetPlayer()->GetPosition();
	// �v���C���[�ƃw���̋���
	float distance = Vector3::Distance(heliPos, playerPos);

	// (�v���C���[���W - �w�����W)����O�i�x�N�g��������o��
	Vector3 forwardVec = playerPos - heliPos;
	forwardVec.Normalize();
	// �s�b�`��]�͂��Ȃ�
	forwardVec.z = 0.0f;

	// �^�[�Q�b�g���W����
	playerPos = playerPos + Vector3(0.0f, 600.0f * m_heli->GetNumber(), 500.0f + 100.0f * m_heli->GetNumber()) + forwardVec * Vector3(-300.0f, 600.0f * m_heli->GetNumber(), 0.0f);
	// �O�i�x�N�g�����čX�V
	forwardVec = playerPos - heliPos;
	forwardVec.Normalize();

	// �v���C���[�����苗�����ꂽ��|�C���g����ɖ߂�
	if (distance > 3001.0f)
	{
		m_heli->SetHeliState(Helicopter::HELI_STATE::PATROL);
		return;
	}

	// �ړ��ʂ��v�Z
	Vector3 move = forwardVec * 150.0f * in_deltaTime;
	
	if (distance > 1000.0f)
	{
		if (distance >= 1800.0f)
		{
			move = forwardVec * 350.0f * in_deltaTime;
		}

		// �w�����ړ�������
		m_owner->SetPosition(heliPos + move);
		forwardVec.z = 0.0f;
		m_owner->RotateToNewForward(forwardVec);

	}
	else
	{
		forwardVec.z = 0.0f;
		m_owner->RotateToNewForward(forwardVec);
		//m_owner->SetPosition(m_owner->GetPosition() + forwardVec * Vector3(-100.0f, 600.0f * m_heli->GetNumber(), 600.0f));
	}

}
