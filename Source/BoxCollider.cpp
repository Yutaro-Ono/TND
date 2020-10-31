//-----------------------------------------------------------------------+
// BoxColliderクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "BoxCollider.h"
#include "Actor.h"
#include "PhysicsWorld.h"
#include "GameMain.h"

// コンストラクタ
BoxCollider::BoxCollider(Actor * in_owner, PhysicsWorld::PhysicsType in_type, int in_updateOrder)
	:Component(in_owner, in_updateOrder)
	,m_objectBox(Vector3::Zero, Vector3::Zero)
	,m_worldBox(Vector3::Zero, Vector3::Zero)
	,m_rotatable(true)
	,m_type(in_type)
{
	GAME_INSTANCE.GetPhysics()->AddBoxCollider(in_type, this);      // BoxColliderをPhysicsWorldに登録
}

// デストラクタ
BoxCollider::~BoxCollider()
{
	printf("Remove BoxCollider : [%5d] owner->( 0x%p )\n", GetID(), m_owner);
	GAME_INSTANCE.GetPhysics()->RemoveBoxCollider(this);
}


void BoxCollider::OnUpdateWorldTransform()
{
	// オブジェクト空間のボックスにリセット
	m_worldBox = m_objectBox;

	// スケーリング
	m_worldBox.m_min *= m_owner->GetScale();
	m_worldBox.m_max *= m_owner->GetScale();

	// 回転
	if (m_rotatable)
	{
		m_worldBox.Rotate(m_owner->GetRotation());
	}

	// 平行移動
	m_worldBox.m_min += m_owner->GetPosition();
	m_worldBox.m_max += m_owner->GetPosition();

}
