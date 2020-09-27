//-----------------------------------------------------------------------+
// ともし火の燭台クラス(リザルト画面で使用).
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "FlameStatue.h"
#include "GameMain.h"
#include "Renderer.h"
#include "MeshComponent.h"


// コンストラクタ
FlameStatue::FlameStatue()
	:m_meshComp(nullptr)
{

}


// デストラクタ
FlameStatue::~FlameStatue()
{
	// 処理なし
}


// メッシュのセット
void FlameStatue::SetMesh(Mesh * in_mesh)
{
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(in_mesh);
}


// 更新処理
void FlameStatue::UpdateActor(float in_deltaTime)
{
}
