#include "PhysicsWorld.h"
#include <typeinfo>
#include <algorithm>
#include "Actor.h"
#include "Player.h"
#include "PlayerCar.h"
#include "PlayerHuman.h"
#include "Collision.h"
#include "BoxCollider.h"

// �R���X�g���N�^
PhysicsWorld::PhysicsWorld()
	:m_boolDebugMode(false)
{
	// �����R���|�[�l���g�z��̊m��
	m_bgBoxes.reserve(256);
	printf("PhysicsWorld : Create\n");
}

PhysicsWorld::~PhysicsWorld()
{
	printf("PhysicsWorld : Delete\n");
	m_bgBoxes.clear();
}

// BoxCollider�̒ǉ�
void PhysicsWorld::AddBoxCollider(PhysicsType in_type, BoxCollider * in_box)
{
	// �v���C���[
	if (in_type == TYPE_PLAYER)
	{
		m_playerBoxes.push_back(in_box);
	}

	// �w�i�����蔻��
	if (in_type == TYPE_BACK_GROUND)
	{
		m_bgBoxes.push_back(in_box);
	}

	// �������蔻��
	if (in_type == TYPE_FLAME)
	{
		m_fires.push_back(in_box);
	}
}

// BoxCollider�̍폜
void PhysicsWorld::RemoveBoxCollider(BoxCollider * in_box)
{
	// BackGround���ɂ��邩
	auto iter = std::find(m_bgBoxes.begin(), m_bgBoxes.end(), in_box);

	if (iter != m_bgBoxes.end())
	{
		m_bgBoxes.erase(iter);
		return;
	}
}

// �f�o�b�O�p�{�b�N�X���X�g�\��
void PhysicsWorld::DebugShowBoxLists()
{
	if (m_bgBoxes.size())
	{
		printf("\n---------------PhysicsList--bgList---------------\n");
		for (auto iter : m_bgBoxes)
		{
			Vector3 pos = iter->GetOwner()->GetPosition();
			printf("6%d ", iter->m_globalID);
			printf("(% 7.2f, % 7.2f % 7.2f)-", pos.x, pos.y, pos.z);
			printf("[%p]\n", iter->GetOwner());
		}
	}
}

void PhysicsWorld::Collision()
{
	// �v���C���[�Ɣw�i�Փ�
	PlayerAndBGTest();
}

void PhysicsWorld::DebugShowBox()
{
}


// �v���C���[�֌W�̓����蔻�菈��
void PhysicsWorld::PlayerAndBGTest()
{
	//�w�i�ƃv���[���[�̏Փˌ��o
	for (auto p : m_playerBoxes)
	{
		for (auto b : m_bgBoxes)
		{
			BoxCollider *player = p;
			BoxCollider *box = b;
			if (Intersect(player->GetWorldBox(), b->GetWorldBox()))
			{
				//�v���[���[�̕ǂ߂荞�ݏC��������
				dynamic_cast<PlayerCar *>(player->GetOwner())->CollisionFix(player, box);
				dynamic_cast<PlayerHuman*>(player->GetOwner())->CollisionFix(player, box);
			}
		}
	}

	// �v���[���[�Ɖ��̏Փˌ��o
	for (auto p : m_playerBoxes)
	{
		for (auto c : m_fires)
		{
			// ���ƃv���[���[���q�b�g�������H
			if (Intersect(p->GetWorldBox(), c->GetWorldBox()))
			{
				c->GetOwner()->SetState(Actor::STATE_DEAD);
			}
		}
	}
}
