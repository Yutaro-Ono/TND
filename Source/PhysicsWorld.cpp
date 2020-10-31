#include "PhysicsWorld.h"
#include <typeinfo>
#include <algorithm>
#include "Actor.h"
#include "PlayerCar.h"
#include "PlayerHuman.h"
#include "ThirdPersonCamera.h"
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
	if (in_type == TYPE_PLAYER_CAR)
	{
		m_playerCarBoxes.push_back(in_box);
	}

	// プレイヤー(人間)
	if (in_type == TYPE_PLAYER_HUMAN)
	{
		m_playerHumanBoxes.push_back(in_box);
	}

	// 背景当たり判定
	if (in_type == TYPE_BACK_GROUND)
	{
		m_bgBoxes.push_back(in_box);
	}

	// 地形当たり判定
	if (in_type == TYPE_TERRAIN)
	{
		m_terrain.push_back(in_box);
	}

	// カメラ当たり判定
	if (in_type == TYPE_CAMERA)
	{
		m_cameraBoxes.push_back(in_box);
	}

}

// BoxColliderの削除
void PhysicsWorld::RemoveBoxCollider(BoxCollider * in_box)
{
	// BackGround内にいるか
	auto box = std::find(m_bgBoxes.begin(), m_bgBoxes.end(), in_box);

	if (box != m_bgBoxes.end())
	{
		m_bgBoxes.erase(box);
		return;
	}

	// 車用当たり判定配列にいるか
	auto car = std::find(m_playerCarBoxes.begin(), m_playerCarBoxes.end(), in_box);

	if (car != m_playerCarBoxes.end())
	{
		m_playerCarBoxes.erase(car);
		return;
	}

	// 人間用当たり判定配列にいるか
	auto human = std::find(m_playerHumanBoxes.begin(), m_playerHumanBoxes.end(), in_box);

	if (human != m_playerHumanBoxes.end())
	{
		m_playerHumanBoxes.erase(human);
		return;
	}

	// カメラ用当たり判定配列にいるか
	auto cam = std::find(m_cameraBoxes.begin(), m_cameraBoxes.end(), in_box);

	if (cam != m_cameraBoxes.end())
	{
		m_cameraBoxes.erase(cam);
		return;
	}

	// 地形用当たり判定配列にあるか
	auto terra = std::find(m_terrain.begin(), m_terrain.end(), in_box);

	if (terra != m_terrain.end())
	{
		m_terrain.erase(terra);
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
	//プレーヤー(車)の衝突検出
	for (auto p : m_playerCarBoxes)
	{
		BoxCollider *playerCar = p;
		
		// 環境オブジェクトとの当たり判定
		for (auto b : m_bgBoxes)
		{
			BoxCollider *box = b;
			if (Intersect(playerCar->GetWorldBox(), b->GetWorldBox()))
			{
				//プレーヤーの壁めり込み修正処理へ
				dynamic_cast<PlayerCar *>(playerCar->GetOwner())->CollisionFix(playerCar, box);
			}
		}

		// 地形との当たり判定
		for (auto t : m_terrain)
		{
			BoxCollider* box = t;
			if (Intersect(playerCar->GetWorldBox(), t->GetWorldBox()))
			{
				//プレーヤーの壁めり込み修正処理へ
				dynamic_cast<PlayerCar*>(playerCar->GetOwner())->CollisionFix(playerCar, box);
			}

		}

	}

	//プレーヤー(人間)の衝突検出
	for (auto p : m_playerHumanBoxes)
	{
		BoxCollider* playerHuman = p;

		// 環境オブジェクトとの当たり判定
		for (auto b : m_bgBoxes)
		{
			BoxCollider* box = b;
			if (Intersect(playerHuman->GetWorldBox(), b->GetWorldBox()))
			{
				//プレーヤーの壁めり込み修正処理へ
				dynamic_cast<PlayerHuman*>(playerHuman->GetOwner())->CollisionFix(playerHuman, box);
			}
		}

		// 地形との当たり判定
		for (auto t : m_terrain)
		{
			BoxCollider* box = t;
			if (Intersect(playerHuman->GetWorldBox(), t->GetWorldBox()))
			{
				//プレーヤーの壁めり込み修正処理へ
				dynamic_cast<PlayerHuman*>(playerHuman->GetOwner())->CollisionFix(playerHuman, box);
			}
		}

		// 車との当たり判定
		for (auto c : m_playerCarBoxes)
		{
			BoxCollider* car = c;
			if (Intersect(playerHuman->GetWorldBox(), c->GetWorldBox()))
			{
				//プレーヤーの壁めり込み修正処理へ
				dynamic_cast<PlayerHuman*>(playerHuman->GetOwner())->CollisionFix(playerHuman, car);
			}
		}
	}

	// カメラの当たり判定
	//for (auto c : m_cameraBoxes)
	//{
	//	BoxCollider* camera = c;

	//	//printf("Min.x : %f, Min.y : %f, Min.z : %f\n", c->m_worldBox.m_min.x, c->m_worldBox.m_min.y, c->m_worldBox.m_min.z);
	//	//printf("Max.x : %f, Max.y : %f, Max.z : %f\n", c->m_worldBox.m_max.x, c->m_worldBox.m_max.y, c->m_worldBox.m_max.z);

	//	// 環境オブジェクトとの当たり判定
	//	for (auto b : m_bgBoxes)
	//	{
	//		BoxCollider* box = b;

	//		// 非表示にしているメッシュを表示状態にする
	//		//b->GetOwner()->GetMeshComponent()->SetVisible(true);

	//		if (Intersect(camera->GetWorldBox(), b->GetWorldBox()))
	//		{
	//			// カメラの壁めり込み時、メッシュを非表示
	//			dynamic_cast<ThirdPersonCamera*>(camera->GetCamera())->CollisionFix(camera, box);

	//		}
	//	}
	//}

}
