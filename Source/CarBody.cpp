#include "CarBody.h"
#include "PlayerCar.h"

// コンストラクタ
CarBody::CarBody(PlayerCar* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	// オーナーの座標をセット
	m_position = m_owner->GetPosition();

	// メッシュのセット
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// ボディのメッシュを基準としてオーナーの当たり判定ボックスをセット
	m_owner->CreateAABB(mesh);
}

// デストラクタ
CarBody::~CarBody()
{
}

void CarBody::UpdateActor(float in_deltaTime)
{
	// オーナーに合わせるためワールド座標を取得し続ける
	m_worldTransform = m_owner->GetWorldTransform();
}
