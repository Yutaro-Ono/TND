#include "CarDoor.h"

const Vector3 CarDoor::ADJUST_DOOR_POSITION = Vector3(0.0f, 0.0f, 0.0f);


CarDoor::CarDoor(PlayerCar* in_owner, const std::string& in_meshPath)
	:m_owner(in_owner)
{
	// オーナーの座標をセット
	m_position = m_owner->GetPosition();

	// メッシュのセット
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);
}

CarDoor::~CarDoor()
{
}

void CarDoor::UpdateActor(float in_deltaTime)
{
	// オーナーに合わせるためワールド座標を取得し続ける
	m_worldTransform = m_owner->GetWorldTransform();

	// 位置を補正
	m_position += ADJUST_DOOR_POSITION;


}
