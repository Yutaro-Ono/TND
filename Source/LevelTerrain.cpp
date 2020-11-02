#include "LevelTerrain.h"
#include "Mesh.h"
#include "BoxCollider.h"
#include "GameMain.h"
#include "Renderer.h"

LevelTerrain::LevelTerrain(Mesh* in_mesh, TERRAIN_TYPE in_type)
	:m_type(in_type)
{
	m_friction = SetTerrainFric(m_type);

	// ���b�V�����Z�b�g
	Mesh* mesh = in_mesh;
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// �����蔻��ݒ�
	m_box = new BoxCollider(this, PhysicsWorld::TYPE_TERRAIN);
	m_box->SetTerrainPtr(this);
	m_box->SetObjectBox(mesh->GetCollisionBox());
	
}

void LevelTerrain::UpdateActor(float in_deltaTime)
{
}

// �n�`�̖��C�͂��Z�b�g
float LevelTerrain::SetTerrainFric(TERRAIN_TYPE in_type)
{
	if (in_type == TERRAIN_TYPE::TYPE_GLASS)
	{
		return 0.3f;
	}

	if (in_type == TERRAIN_TYPE::TYPE_STREET)
	{
		return 1.2f;
	}


	return 1.0f;
}
