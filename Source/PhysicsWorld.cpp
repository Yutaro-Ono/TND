#include "PhysicsWorld.h"
#include <typeinfo>
#include <algorithm>
#include "Actor.h"
#include "Player.h"
#include "PlayerCar.h"
#include "PlayerHuman.h"
#include "ThirdPersonCamera.h"
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
	if (in_type == TYPE_PLAYER_CAR)
	{
		m_playerCarBoxes.push_back(in_box);
	}

	// �v���C���[(�l��)
	if (in_type == TYPE_PLAYER_HUMAN)
	{
		m_playerHumanBoxes.push_back(in_box);
	}

	// �w�i�����蔻��
	if (in_type == TYPE_BACK_GROUND)
	{
		m_bgBoxes.push_back(in_box);
	}

	// �J���������蔻��
	if (in_type == TYPE_CAMERA)
	{
		m_cameraBoxes.push_back(in_box);
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
	//�v���[���[(��)�̏Փˌ��o
	for (auto p : m_playerCarBoxes)
	{
		BoxCollider *playerCar = p;
		
		// ���I�u�W�F�N�g�Ƃ̓����蔻��
		for (auto b : m_bgBoxes)
		{
			BoxCollider *box = b;
			if (Intersect(playerCar->GetWorldBox(), b->GetWorldBox()))
			{
				//�v���[���[�̕ǂ߂荞�ݏC��������
				dynamic_cast<PlayerCar *>(playerCar->GetOwner())->CollisionFix(playerCar, box);
			}
		}

	}

	//�v���[���[(�l��)�̏Փˌ��o
	for (auto p : m_playerHumanBoxes)
	{
		BoxCollider* playerHuman = p;

		// ���I�u�W�F�N�g�Ƃ̓����蔻��
		for (auto b : m_bgBoxes)
		{
			BoxCollider* box = b;
			if (Intersect(playerHuman->GetWorldBox(), b->GetWorldBox()))
			{
				//�v���[���[�̕ǂ߂荞�ݏC��������
				dynamic_cast<PlayerHuman*>(playerHuman->GetOwner())->CollisionFix(playerHuman, box);
			}
		}

		// �ԂƂ̓����蔻��
		for (auto c : m_playerCarBoxes)
		{
			BoxCollider* car = c;
			if (Intersect(playerHuman->GetWorldBox(), c->GetWorldBox()))
			{
				//�v���[���[�̕ǂ߂荞�ݏC��������
				dynamic_cast<PlayerHuman*>(playerHuman->GetOwner())->CollisionFix(playerHuman, car);
			}
		}
	}

	// �J�����̓����蔻��
	for (auto c : m_cameraBoxes)
	{
		BoxCollider* camera = c;

		//printf("Min.x : %f, Min.y : %f, Min.z : %f\n", c->m_worldBox.m_min.x, c->m_worldBox.m_min.y, c->m_worldBox.m_min.z);
		//printf("Max.x : %f, Max.y : %f, Max.z : %f\n", c->m_worldBox.m_max.x, c->m_worldBox.m_max.y, c->m_worldBox.m_max.z);

		// ���I�u�W�F�N�g�Ƃ̓����蔻��
		for (auto b : m_bgBoxes)
		{
			BoxCollider* box = b;
			if (Intersect(camera->GetWorldBox(), b->GetWorldBox()))
			{
				//�v���[���[�̕ǂ߂荞�ݏC��������
				dynamic_cast<ThirdPersonCamera*>(camera->GetCamera())->CollisionFix(camera, box);

			}
		}
	}

}
