//-----------------------------------------------------+
// 環境オブジェクトクラス (当たり判定無し)
//-----------------------------------------------------+
#include "LevelObject.h"
#include "GameMain.h"
#include "Renderer.h"
#include "MeshComponent.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="in_mesh">メッシュファイルのパス</param>
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
