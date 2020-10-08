#include "PhysicsWorld.h"
#include <typeinfo>
#include <algorithm>
#include "Actor.h"
#include "Player.h"
#include "PlayerCar.h"
#include "PlayerHuman.h"
#include "Collision.h"
#include "BoxCollider.h"

// コンストラクタ
PhysicsWorld::PhysicsWorld()
	:m_boolDebugMode(false)
{
	// 物理コンポーネント配列の確保
	m_bgBoxes.reserve(256);
	printf("PhysicsWorld : Create\n");
}

PhysicsWorld::~PhysicsWorld()
{
	printf("PhysicsWorld : Delete\n");
	m_bgBoxes.clear();
}

// BoxColliderの追加
void PhysicsWorld::AddBoxCollider(PhysicsType in_type, BoxCollider * in_box)
{
	// プレイヤー
	if (in_type == TYPE_PLAYER)
	{
		m_playerBoxes.push_back(in_box);
	}

	// 背景当たり判定
	if (in_type == TYPE_BACK_GROUND)
	{
		m_bgBoxes.push_back(in_box);
	}

	// 炎当たり判定
	if (in_type == TYPE_FLAME)
	{
		m_fires.push_back(in_box);
	}
}

// BoxColliderの削除
void PhysicsWorld::RemoveBoxCollider(BoxCollider * in_box)
{
	// BackGround内にいるか
	auto iter = std::find(m_bgBoxes.begin(), m_bgBoxes.end(), in_box);

	if (iter != m_bgBoxes.end())
	{
		m_bgBoxes.erase(iter);
		return;
	}
}

// デバッグ用ボックスリスト表示
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
	// プレイヤーと背景衝突
	PlayerAndBGTest();
}

void PhysicsWorld::DebugShowBox()
{
}


// プレイヤー関係の当たり判定処理
void PhysicsWorld::PlayerAndBGTest()
{
	//背景とプレーヤーの衝突検出
	for (auto p : m_playerBoxes)
	{
		for (auto b : m_bgBoxes)
		{
			BoxCollider *player = p;
			BoxCollider *box = b;
			if (Intersect(player->GetWorldBox(), b->GetWorldBox()))
			{
				//プレーヤーの壁めり込み修正処理へ
				dynamic_cast<PlayerCar *>(player->GetOwner())->CollisionFix(player, box);
				dynamic_cast<PlayerHuman*>(player->GetOwner())->CollisionFix(player, box);
			}
		}
	}

	// プレーヤーと炎の衝突検出
	for (auto p : m_playerBoxes)
	{
		for (auto c : m_fires)
		{
			// 炎とプレーヤーがヒットしたか？
			if (Intersect(p->GetWorldBox(), c->GetWorldBox()))
			{
				c->GetOwner()->SetState(Actor::STATE_DEAD);
			}
		}
	}
}
