//-----------------------------------------------------------------------+
// 天使像オブジェクト.
// 
// 
// copyright (C) 2020 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "AngelStatue.h"
#include "MeshComponent.h"


// コンストラクタ
AngelStatue::AngelStatue()
	:m_meshComp(nullptr)
{
	// 処理なし
}


// デストラクタ
AngelStatue::~AngelStatue()
{
	// 処理なし
}


// メッシュのセット
void AngelStatue::SetMesh(Mesh * in_mesh)
{
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(in_mesh);
}


// 更新処理
void AngelStatue::UpdateActor(float in_deltaTime)
{
	// 処理なし
}
