#include "CarWheel.h"

CarWheel::CarWheel(PlayerCar* in_owner, const std::string& in_meshPath, WHEEL_POSITION in_enumPos)
	:m_owner(in_owner)
	,m_wheelPosition(in_enumPos)
{

	m_position = m_owner->GetPosition();

	// メッシュのセット
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	// タイヤ位置ごとの座標調整
	switch (m_wheelPosition)
	{

	case WHEEL_POSITION::FRONT_LEFT:

		m_adjustPos = Vector3(140.0f, -80.0f, 40.0f);

		break;

	case WHEEL_POSITION::FRONT_RIGHT:

		m_adjustPos = Vector3(140.0f, 103.0f, 40.0f);

		break;

	case WHEEL_POSITION::BACK_LEFT:

		m_adjustPos = Vector3(-150.0f, -80.0f, 40.0f);

		break;

	case WHEEL_POSITION::BACK_RIGHT:

		m_adjustPos = Vector3(-150.0f, 103.0f, 40.0f);

		break;


	default:

		break;
	}
}

CarWheel::~CarWheel()
{
}

void CarWheel::UpdateActor(float in_deltaTime)
{
	// オーナーの拡大率・座標・回転行列に合わせておく
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + m_adjustPos;
	m_rotation = m_owner->GetRotation();


	// 回転値
	static float spin = 0.0f;

	// オーナーであるPlayerCarクラスの運転状態がアクセル状態であれば
	// タイヤを前方に回す
	if (m_owner->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_ACCEL)
	{
		spin += 30.0f * in_deltaTime;
		m_rotation = Quaternion::Quaternion(Vector3::UnitY, spin);
	}
	// ブレーキ状態なら後ろに回す
	if (m_owner->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_BRAKE)
	{
		spin -= 30.0f * in_deltaTime;
		m_rotation = Quaternion::Quaternion(Vector3::UnitY, spin);
	}

	// オーナーに合わせるためワールド座標を取得し続ける
	m_worldTransform = Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();

}
