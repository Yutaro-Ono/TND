//-----------------------------------------------------+
// ���I�u�W�F�N�g�N���X (�����蔻�薳��)
//-----------------------------------------------------+
#include "LevelObject.h"
#include "GameMain.h"
#include "Renderer.h"
#include "MeshComponent.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="in_mesh">���b�V���t�@�C���̃p�X</param>
LevelObject::LevelObject(Mesh * in_mesh)
{
	Mesh* mesh = in_mesh;
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
	m_meshComp->SetMapColor(Vector3(0.25f, 0.25f, 0.3f));
}

LevelObject::~LevelObject()
{
}
