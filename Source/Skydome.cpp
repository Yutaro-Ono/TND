//-----------------------------------------------------------------------+
// スカイドームクラス.
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Skydome.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"


// スカイドーム
Skydome::Skydome()
	:m_radian(0.0f)
	,m_isSpin(false)
{

	m_position = Vector3(0.0f, 0.0f, 0.0f);

}


// デストラクタ
Skydome::~Skydome()
{
	// 処理なし
}


// メッシュのセター
void Skydome::SetMesh(Mesh * in_mesh)
{

	// メッシュをセット
	Mesh* mesh = in_mesh;
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

}


// 更新処理
void Skydome::UpdateActor(float in_deltaTime)
{

	// 回転させる
	if (m_isSpin == true)
	{
		m_radian += 0.03f * in_deltaTime;

		SetRotation(Quaternion(Vector3::UnitZ, m_radian));

		m_rotation.Normalize();
	}

}
