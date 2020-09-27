//-----------------------------------------------------------------------+
// BoxColliderクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"

class BoxCollider : public Component
{
public:

	BoxCollider(class Actor* in_owner, PhysicsWorld::PhysicsType in_type, int in_updateOrder = 100);
	~BoxCollider();

	void OnUpdateWorldTransform() override;                                 // ワールド変換時
	void SetObjectBox(const AABB& in_box) { m_objectBox = in_box; }         // 当たり判定用境界ボックスをセット
	const AABB& GetWorldBox() const { return m_worldBox; }                  // ワールド空間上での境界ボックスを取得
	void setArrowRotate(bool in_value) { m_rotatable = in_value; }          // 回転を許可するか

private:

	AABB m_objectBox;                                                       // オブジェクト空間(変換前)のボックス
	AABB m_worldBox;                                                        // ワールド空間でのボックス

	bool m_rotatable;                                                       // 回転を許可するか

	friend class PhysicsWorld;                                              // フレンド化

};