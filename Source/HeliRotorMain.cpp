#include "HeliRotorMain.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Helicopter.h"

static float radian = 0.0f;

HeliRotorMain::HeliRotorMain(Helicopter* in_heli, const std::string& in_filePath)
	:m_owner(in_heli)
{

	// メッシュのロード・レンダラーへの登録
	Mesh* mesh = RENDERER->GetMesh(in_filePath);
	// メッシュコンポーネントにロードしたメッシュをセット
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
	m_meshComp->SetMapColor(Vector3(0.8f, 0.45f, 0.75f));

	// オーナーに座標・スケール・回転・ワールド行列を合わせる
	SetScale(m_owner->GetScale());
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());
	SetWorldTransform(m_owner->GetWorldTransform());
}

HeliRotorMain::~HeliRotorMain()
{
}

void HeliRotorMain::UpdateActor(float in_deltaTime)
{
	// オーナーに座標・スケール・回転・ワールド行列を合わせる
	SetScale(m_owner->GetScale());
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());

	// プロペラ回転
	radian += 30.0f * Math::Pi * in_deltaTime;
	if (radian >= 360000.0f)
	{
		radian = 0.0f;
	}

	SetRotation(Quaternion::Quaternion(Vector3::UnitZ, Math::ToRadians(radian)));

	SetWorldTransform(m_owner->GetWorldTransform());
}
