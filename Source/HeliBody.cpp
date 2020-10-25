#include "HeliBody.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Helicopter.h"

HeliBody::HeliBody(Helicopter* in_heli, const std::string& in_filePath)
{
	// オーナークラスをセット
	m_owner = in_heli;

	// オーナーに座標・スケール・回転・ワールド行列を合わせる
	SetScale(m_owner->GetScale());
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());
	// メッシュのロード・レンダラーへの登録
	Mesh* mesh = RENDERER->GetMesh(in_filePath);
	// メッシュコンポーネントにロードしたメッシュをセット
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

}

HeliBody::~HeliBody()
{
}

void HeliBody::UpdateActor(float in_deltaTime)
{
	// オーナーに座標・スケール・回転・ワールド行列を合わせる
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());
	SetWorldTransform(m_owner->GetWorldTransform());
}
