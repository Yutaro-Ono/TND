#include "TitleCarHandle.h"
#include "Mesh.h"
#include "MeshComponent.h"

TitleCarHandle::TitleCarHandle(Actor* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	// オーナーの座標をセット
	m_position = m_owner->GetPosition();

	// メッシュのセット
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
}

TitleCarHandle::~TitleCarHandle()
{
}

void TitleCarHandle::UpdateActor(float in_deltaTime)
{
	// オーナーの拡大率・座標・回転行列に合わせておく
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + Vector3(20.0f, 0.0f, 30.0f);
	m_rotation = m_owner->GetRotation();

	Matrix4 handleMat = Matrix4::CreateScale(m_scale) * Matrix4::CreateFromQuaternion(m_rotation) * Matrix4::CreateTranslation(m_position);

	// オーナーに合わせるためワールド座標を取得し続ける
	m_worldTransform = handleMat;

}
