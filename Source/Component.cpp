//-----------------------------------------------------------------------+
// �R���|�[�l���g�N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Component.h"
#include "Actor.h"

int Component::m_globalID = 0;

// �R���X�g���N�^
Component::Component(Actor* in_owner, int in_updateOrder)
	:m_owner(in_owner)
	,m_updateOrder(in_updateOrder)
{
	// �A�N�^�[�ɂ��̃R���|�[�l���g��ǉ�
	m_owner->AddComponent(this);
	m_ID = m_globalID;
	m_globalID++;
}

// �f�X�g���N�^
Component::~Component()
{
	m_owner->RemoveComponent(this);
}

void Component::Update(float in_deltaTime)
{
}
