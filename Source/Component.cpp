//-----------------------------------------------------------------------+
// コンポーネントクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Component.h"
#include "Actor.h"

int Component::m_globalID = 0;

// コンストラクタ
Component::Component(Actor* in_owner, int in_updateOrder)
	:m_owner(in_owner)
	,m_updateOrder(in_updateOrder)
{
	// アクターにこのコンポーネントを追加
	m_owner->AddComponent(this);
	m_ID = m_globalID;
	m_globalID++;
}

// デストラクタ
Component::~Component()
{
	m_owner->RemoveComponent(this);
}

void Component::Update(float in_deltaTime)
{
}
