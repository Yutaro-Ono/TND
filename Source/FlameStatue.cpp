//-----------------------------------------------------------------------+
// �Ƃ����΂̐C��N���X(���U���g��ʂŎg�p).
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "FlameStatue.h"
#include "GameMain.h"
#include "Renderer.h"
#include "MeshComponent.h"


// �R���X�g���N�^
FlameStatue::FlameStatue()
	:m_meshComp(nullptr)
{

}


// �f�X�g���N�^
FlameStatue::~FlameStatue()
{
	// �����Ȃ�
}


// ���b�V���̃Z�b�g
void FlameStatue::SetMesh(Mesh * in_mesh)
{
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(in_mesh);
}


// �X�V����
void FlameStatue::UpdateActor(float in_deltaTime)
{
}
