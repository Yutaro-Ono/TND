#include "CarWheel.h"
#include "SandSmoke.h"


// ��]�l
static float spin = 0.0f;

const std::string CarWheel::CAR_WHEEL_RUBBER_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Wheel/WheelRubber_Internal.OBJ";
const std::string CarWheel::CAR_WHEEL_STEEL_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Wheel/WheelSteel_Internal.OBJ";


CarWheel::CarWheel(PlayerCar* in_owner, WHEEL_POSITION in_enumPos)
	:m_owner(in_owner)
	,m_smoke(nullptr)
	,m_wheelPosition(in_enumPos)
	,m_playerRadian(m_owner->GetRadian())
	,m_spin(false)
{

	m_position = m_owner->GetPosition();

	// ���b�V���̃Z�b�g
	Mesh* rubberMesh = RENDERER->GetMesh(CAR_WHEEL_RUBBER_MESH_PATH);
	Mesh* steelMesh = RENDERER->GetMesh(CAR_WHEEL_STEEL_MESH_PATH);
	MeshComponent* rubberMeshComp = new MeshComponent(this);
	MeshComponent* steelMeshComp = new MeshComponent(this);
	rubberMeshComp->SetMesh(rubberMesh);
	rubberMeshComp->SetDrawMap(false);
	rubberMeshComp->SetMapColor(Vector3(0.45f, 0.8f, 0.75f));
	steelMeshComp->SetMesh(steelMesh);
	steelMeshComp->SetDrawMap(false);

	// �^�C���ʒu���Ƃ̍��W����
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

	// �����p�[�e�B�N������
	m_smoke = new SandSmoke(this);
}

CarWheel::~CarWheel()
{
}

void CarWheel::UpdateActor(float in_deltaTime)
{
	// �I�[�i�[�̊g�嗦�E���W�E��]�s��ɍ��킹�Ă���
	m_scale = m_owner->GetScale();
	m_position = m_owner->GetPosition() + m_adjustPos;
	m_rotation = m_owner->GetRotation();

	//static float dir = 0.0f;
	Vector3 dir = Vector3::Zero;

	// �^�C����Z����]����
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
	// �I�[�i�[�ł���PlayerCar�N���X�̉^�]��Ԃ��A�N�Z����Ԃł����
	// �^�C����O���ɉ�
	if (m_owner->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_ACCEL && m_spin == true)
	{
		spin += m_owner->GetMoveComponent()->GetAccelValue() * 3.0f * in_deltaTime;
	}
	// �u���[�L��ԂȂ���ɉ�
	if (m_owner->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_BRAKE && m_spin == true)
	{
		spin -= m_owner->GetMoveComponent()->GetBrakeValue() * 3.0f * in_deltaTime;
	}


	
	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾�������� (�O���̃^�C����Z���ɂ���]������)
	if (m_wheelPosition == WHEEL_POSITION::FRONT_LEFT || m_wheelPosition == WHEEL_POSITION::FRONT_RIGHT)
	{
		m_worldTransform = Matrix4::CreateRotationY(Math::ToRadians(spin)) * Matrix4::CreateRotationZ(dir.z) * Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();
	}
	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾��������
	else
	{
		m_worldTransform = Matrix4::CreateRotationY(Math::ToRadians(spin)) * Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();

	}

	m_playerRadian = m_owner->GetMoveComponent()->GetRadian();
}
