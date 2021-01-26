#include "HeliRotorBack.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Helicopter.h"

static float radian = 0.0f;
//const Vector3 ADJUST_POS = Vector3(-320.0f, -10.0f, 95.0f);

HeliRotorBack::HeliRotorBack(Helicopter* in_heli, const std::string& in_filePath)
{
	// オーナークラスをセット
	m_owner = in_heli;

	// メッシュのロード・レンダラーへの登録
	Mesh* mesh = RENDERER->GetMesh(in_filePath);
	// メッシュコンポーネントにロードしたメッシュをセット
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
	m_meshComp->SetMapColor(Vector3(0.8f, 0.45f, 0.75f));
	// オーナーに座標・スケール・回転を合わせる
	SetScale(m_owner->GetScale());
	SetPosition(m_owner->GetPosition());
	SetRotation(m_owner->GetRotation());

}

HeliRotorBack::~HeliRotorBack()
{
}

void HeliRotorBack::UpdateActor(float in_deltaTime)
{
	const Vector3 ADJUST_POS = Vector3(-320.0f, -10.0f, 95.0f);

	// オーナーに座標・回転・ワールド行列を合わせる
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + ADJUST_POS;
	m_rotation = m_owner->GetRotation();

	// プロペラ回転
	radian += 20.0f * in_deltaTime;
	if (radian >= 360.0f)
	{
		radian = 0.0f;
	}
	Quaternion rot = Quaternion::Quaternion(Vector3::UnitY, radian);

	// 位置調整ベクトルを回転に応じて行進
	Vector3 offset = ADJUST_POS;
	offset = Vector3::Transform(offset, m_rotation);
	// ワールド変換行列更新
	m_worldTransform = Matrix4::CreateFromQuaternion(rot) * m_owner->GetWorldTransform() * Matrix4::CreateTranslation(offset);

}
