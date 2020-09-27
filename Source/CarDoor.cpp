#include "CarDoor.h"


// in_leftRight = 右側のドアか左側のドアかで位置補正ベクトルを変える
// in_leftRight = true → 左側     false → 右側
CarDoor::CarDoor(PlayerCar* in_owner, const std::string& in_meshPath, bool in_leftRight)
	:m_owner(in_owner)
{



	if (in_leftRight)
	{
		m_adjustPos = Vector3(20.0f, -100.0f, 100.0f);
	}
	else
	{
		m_adjustPos = Vector3(20.0f, 100.0f, 100.0f);
	}

	//// 拡大率もオーナーに合わせる
	m_scale = m_owner->GetScale();
	// オーナーの座標をセット
	m_position = m_owner->GetPosition() + m_adjustPos;

	m_rotation = m_owner->GetRotation();

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
	// オーナーの拡大率・座標・回転行列に合わせておく
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + m_adjustPos;
	m_rotation = m_owner->GetRotation();

	// オーナーに合わせるためワールド座標を取得し続ける (補正ベクトルで位置を補正)
	m_worldTransform = Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();

}
