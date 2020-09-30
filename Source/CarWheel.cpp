#include "CarWheel.h"

CarWheel::CarWheel(PlayerCar* in_owner, const std::string& in_meshPath, WHEEL_POSITION in_enumPos)
	:m_owner(in_owner)
	,m_wheelPosition(in_enumPos)
{

	m_position = m_owner->GetPosition();

	// ���b�V���̃Z�b�g
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh(in_meshPath);
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

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


	// ��]�l
	static float spin = 0.0f;

	// �I�[�i�[�ł���PlayerCar�N���X�̉^�]��Ԃ��A�N�Z����Ԃł����
	// �^�C����O���ɉ�
	if (m_owner->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_ACCEL)
	{
		spin += 30.0f * in_deltaTime;
		m_rotation = Quaternion::Quaternion(Vector3::UnitY, spin);
	}
	// �u���[�L��ԂȂ���ɉ�
	if (m_owner->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_BRAKE)
	{
		spin -= 30.0f * in_deltaTime;
		m_rotation = Quaternion::Quaternion(Vector3::UnitY, spin);
	}

	// �I�[�i�[�ɍ��킹�邽�߃��[���h���W���擾��������
	m_worldTransform = Matrix4::CreateTranslation(m_adjustPos) * m_owner->GetWorldTransform();

}
