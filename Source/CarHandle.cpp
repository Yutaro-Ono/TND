#include "CarHandle.h"

CarHandle::CarHandle(PlayerCar* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	SetScale(m_owner->GetScale());
	// メッシュのセット
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
}

CarHandle::~CarHandle()
{
}

void CarHandle::UpdateActor(float in_deltaTime)
{

	// 行列計算
	// 親クラスと位置を合わせたうえで回転させる必要があるため
	// ここで計算する。(fix)
	Vector3 adjustPos = Vector3(18.0f, -15.0f, 30.0f);

	// 拡大率行列
	Matrix4 scale = Matrix4::CreateScale(m_owner->GetScale());
	// 回転行列
	Matrix4 rot = Matrix4::CreateFromQuaternion(m_owner->GetRotation());
	// 調整用の座標行列 (回転行列による変換をする)
	Matrix4 adjust = Matrix4::CreateTranslation(Vector3::Transform(adjustPos, rot));
	// ワールド変換行列
	Matrix4 trns = Matrix4::CreateTranslation(m_owner->GetPosition());

	m_worldTransform = scale * rot * adjust * trns;

}
