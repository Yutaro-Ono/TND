#include "HeliSpotLight.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Helicopter.h"

HeliSpotLight::HeliSpotLight(Helicopter* in_heli, const std::string& in_filePath)
{
	// オーナークラスをセット
	m_owner = in_heli;

	// メッシュのロード・レンダラーへの登録
	Mesh* mesh = RENDERER->GetMesh(in_filePath);
	// メッシュコンポーネントにロードしたメッシュをセット
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// オーナーに座標・スケール・回転・ワールド行列を合わせる
	SetScale(m_owner->GetScale());
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());
	SetWorldTransform(m_owner->GetWorldTransform());
}

HeliSpotLight::~HeliSpotLight()
{
}

void HeliSpotLight::UpdateActor(float in_deltaTime)
{
	// オーナーに座標・スケール・回転・ワールド行列を合わせる
	SetScale(m_owner->GetScale());
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());
	SetWorldTransform(m_owner->GetWorldTransform());
}
