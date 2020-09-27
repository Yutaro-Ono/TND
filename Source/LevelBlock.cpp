//-----------------------------------------------------------------------+
// �X�e�[�W�p�u���b�N�N���X.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "LevelBlock.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "BoxCollider.h"


LevelBlock::LevelBlock()
	:m_meshComp(nullptr)
{
	m_position = Vector3(0.0f, 0.0f, 0.0f);
}

LevelBlock::~LevelBlock()
{
	
}

void LevelBlock::SetMesh(Mesh * in_mesh)
{
	// ���b�V�����Z�b�g
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(in_mesh);

	 // �����蔻��ݒ�
	 m_box = new BoxCollider(this, PhysicsWorld::TYPE_BACK_GROUND);
	 m_box->SetObjectBox(in_mesh->GetCollisionBox());
}
