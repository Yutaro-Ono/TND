//-----------------------------------------------------------------------+
// �X�J�C�h�[���N���X.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Skydome.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"


// �X�J�C�h�[��
Skydome::Skydome()
	:m_radian(0.0f)
	,m_isSpin(false)
{

	m_position = Vector3(0.0f, 0.0f, 0.0f);

}


// �f�X�g���N�^
Skydome::~Skydome()
{
	// �����Ȃ�
}


// ���b�V���̃Z�^�[
void Skydome::SetMesh(Mesh * in_mesh)
{

	// ���b�V�����Z�b�g
	Mesh* mesh = in_mesh;
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

}


// �X�V����
void Skydome::UpdateActor(float in_deltaTime)
{

	// ��]������
	if (m_isSpin == true)
	{
		m_radian += 0.03f * in_deltaTime;

		SetRotation(Quaternion(Vector3::UnitZ, m_radian));

		m_rotation.Normalize();
	}

}
