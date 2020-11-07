#pragma once
#include "Actor.h"
#include "MeshComponent.h"


class LevelTerrain : public Actor
{

public:


	enum TERRAIN_TYPE
	{
		TYPE_GLASS = 0,
		TYPE_STREET
	};



	LevelTerrain(class Mesh* in_mesh, TERRAIN_TYPE in_type);
	~LevelTerrain();

	void UpdateActor(float in_deltaTime) override;

	float SetTerrainFric(TERRAIN_TYPE in_type);

	TERRAIN_TYPE GetNodeType() { return m_type; }

	float GetFrictionVal() { return m_friction; }

	void SetMeshVisible() { m_meshComp->SetVisible(false); }     // ���b�V���̓�����

private:


	TERRAIN_TYPE m_type;           // �n�`�^�C�v

	float m_friction;          // ���C�l (0 �` 1.0)�Ⴂ�قǒn�`��ł̈ړ����x���Ȃ�

	class BoxCollider* m_box;     // �����蔻��{�b�N�X
};