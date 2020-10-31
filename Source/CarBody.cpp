#include "CarBody.h"
#include "PlayerCar.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"

// コンストラクタ
CarBody::CarBody(PlayerCar* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	// オーナーの拡大率・座標・回転行列に合わせておく
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();
	// メッシュのセット
	Mesh* mesh = RENDERER->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// ボディのメッシュを基準としてオーナーの当たり判定ボックスをセット
	// 当たり判定ボックスのセット
	AABB playerBox = mesh->GetCollisionBox();
	m_owner->SetBoxCollider(new BoxCollider(in_owner, PhysicsWorld::TYPE_PLAYER_CAR));
	//playerBox.m_min.x *= 0.6f;
	//playerBox.m_min.y *= 0.6f;
	//playerBox.m_max.x *= 0.6f;
	//playerBox.m_max.y *= 0.6f;
	playerBox.m_min.z *= 0.05f;

	playerBox.m_max.z *= 1.0f;
	m_owner->GetBoxCollider()->SetObjectBox(playerBox);
}

// デストラクタ
CarBody::~CarBody()
{
}

void CarBody::UpdateActor(float in_deltaTime)
{
	// オーナーの拡大率・座標・回転行列に合わせておく
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition();
	m_rotation = m_owner->GetRotation();

	// オーナーに合わせるためワールド座標を取得し続ける
	m_worldTransform = m_owner->GetWorldTransform();
}
