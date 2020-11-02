#include "CarWheel.h"

// 回転値
static float spin = 0.0f;

CarWheel::CarWheel(PlayerCar* in_owner, const std::string& in_meshPath, WHEEL_POSITION in_enumPos)
	:m_owner(in_owner)
	,m_wheelPosition(in_enumPos)
	,m_playerRadian(m_owner->GetRadian())
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



	//static float dir = 0.0f;
	Vector3 dir = Vector3::Zero;

	// タイヤのZ軸回転処理
	if (m_owner->GetTurnState() == PlayerCar::TURN_LEFT)
	{
		//dir = -0.25f;
		dir = Vector3::Lerp(dir, Vector3(0.0f, 0.0f, -3.0f), 2.0f * in_deltaTime);
	}
	if (m_owner->GetTurnState() == PlayerCar::TURN_RIGHT)
	{
		//dir = 0.25f;
		dir = Vector3::Lerp(dir, Vector3(0.0f, 0.0f, 3.0f), 2.0f * in_deltaTime);
	}

	if (spin > 10000000.0f || spin < -10000000.0f)
	{
		spin = 0.0f;
	}
	// オーナーであるPlayerCarクラスの運転状態がアクセル状態であれば
	// タイヤを前方に回す
	if (m_owner->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_ACCEL)
	{
		spin += m_owner->GetMoveComponent()->GetAccelValue() * 3.0f * in_deltaTime;
	}
	// ブレーキ状態なら後ろに回す
	if (m_owner->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_BRAKE)
	{
		spin -= m_owner->GetMoveComponent()->GetBrakeValue() * 3.0f * in_deltaTime;
	}


	
	// オーナーに合わせるためワールド座標を取得し続ける (前方のタイヤはZ軸にも回転させる)
	if (m_wheelPosition == WHEEL_POSITION::FRONT_LEFT || m_wheelPosition == WHEEL_POSITION::FRONT_RIGHT)
	{
		m_worldTransform = Matrix4::CreateRotationY(Math::ToRadians(spin)) * Matrix4::CreateRotationZ(dir.z) * Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();
	}
	// オーナーに合わせるためワールド座標を取得し続ける
	else
	{
		m_worldTransform = Matrix4::CreateRotationY(Math::ToRadians(spin)) * Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();

	}

	m_playerRadian = m_owner->GetMoveComponent()->GetRadian();
}
