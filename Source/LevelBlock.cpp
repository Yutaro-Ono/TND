//-----------------------------------------------------------------------+
// ステージ用ブロッククラス.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "LevelBlock.h"
#include "Mesh.h"
#include "BoxCollider.h"
#include "GameMain.h"
#include "Renderer.h"
#include "CameraComponent.h"

LevelBlock::LevelBlock()
{
	m_position = Vector3(0.0f, 0.0f, 0.0f);
}

LevelBlock::~LevelBlock()
{
	
}

void LevelBlock::SetMesh(Mesh * in_mesh)
{
	// メッシュをセット
	Mesh* mesh = in_mesh;
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
	m_meshComp->SetMapColor(Vector3(0.3f, 0.35f, 0.4f));


	 // 当たり判定設定
	 m_box = new BoxCollider(this, PhysicsWorld::TYPE_BACK_GROUND);
	 m_box->SetObjectBox(mesh->GetCollisionBox());
}

void LevelBlock::UpdateActor(float in_deltaTime)
{

}
