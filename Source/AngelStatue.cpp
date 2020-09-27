//-----------------------------------------------------------------------+
// �V�g���I�u�W�F�N�g.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "AngelStatue.h"
#include "MeshComponent.h"


// �R���X�g���N�^
AngelStatue::AngelStatue()
	:m_meshComp(nullptr)
{
	// �����Ȃ�
}


// �f�X�g���N�^
AngelStatue::~AngelStatue()
{
	// �����Ȃ�
}


// ���b�V���̃Z�b�g
void AngelStatue::SetMesh(Mesh * in_mesh)
{
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(in_mesh);
}


// �X�V����
void AngelStatue::UpdateActor(float in_deltaTime)
{
	// �����Ȃ�
}
