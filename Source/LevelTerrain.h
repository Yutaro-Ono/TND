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

	void SetMeshVisible() { m_meshComp->SetVisible(false); }     // メッシュの透明化

private:


	TERRAIN_TYPE m_type;           // 地形タイプ

	float m_friction;          // 摩擦値 (0 ～ 1.0)低いほど地形上での移動が遅くなる

	class BoxCollider* m_box;     // 当たり判定ボックス
};