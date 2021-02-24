#include "MoveComponentCar.h"
#include "InputController.h"
#include "Input.h"
#include "Actor.h"
#include "PlayerCar.h"

const float MoveComponentCar::HAND_BRAKE_VALUE = 8.0f;            // �n���h�u���[�L���쎞�̃u���[�L�l
const float MoveComponentCar::ACCEL_LIMIT = 150.0f;                // �ő呬�x�̏���l(���݂̊�F150km/h)
const float MoveComponentCar::BRAKE_LIMIT = 35.0f;

// �R���X�g���N�^
MoveComponentCar::MoveComponentCar(PlayerCar* in_owner)
	:MoveComponent(in_owner)
	,m_accelValue(0.0f)
	,m_brakeValue(0.0f)
	,m_handBrake(0.0f)
	,m_accelLimit(ACCEL_LIMIT)
	,m_brakeLimit(BRAKE_LIMIT)
	,m_radian(0.0f)
	,m_pastRadian(m_radian)
{
	m_playerCar = in_owner;
}

// �f�X�g���N�^
MoveComponentCar::~MoveComponentCar()
{
}

void MoveComponentCar::Update(float in_deltaTime)
{
	if (m_isActive)
	{
		if (m_padIsActive)
		{
			MovementByController(in_deltaTime);
		}
		else
		{
			MovementByKeyboard(in_deltaTime);
		}
	}

	// �d�͏���
	// m_owner->SetPosition(m_owner->GetPosition() - Vector3(0.0f, 0.0f, 1.0f));
	if (m_owner->GetPosition().z <= -20.0f)
	{
		m_owner->SetPosition(m_owner->GetPosition() + Vector3(0.0f, 0.0f, 20.0f));
	}
}


void MoveComponentCar::MovementByController(float in_deltaTime)
{
	//�L��������
	const float speed = 2.0f;
	Vector3 forwardVec = Vector3(60.0f, 0.0f, 0.0f);
	Vector3 rightVec = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = m_owner->GetForward();

	// �L�����N�^�[�ړ�
	Vector3 DirVec(0.0f, 0.0f, 0.0f);

	// �A�N�Z�����E�l
	if (m_accelValue > ACCEL_LIMIT * m_playerCar->GetFrictionVal())
	{
		m_accelValue = ACCEL_LIMIT * m_playerCar->GetFrictionVal();
	}
	//---------------------------------------------------------------------------------------------+
	// �v���C���[�̑���
	//---------------------------------------------------------------------------------------------+
	// �R���g���[���[�ڑ����Ɩ��ڑ����ŏ����𕪂���
			// �E�g���K�[�A���g���K�[�擾
	float triggerR = CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	float triggerL = CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	// ���X�e�B�b�N���͒l���擾
	Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();

	// �g���K�[���͒l��\��
	// printf("R_TRIGGER : %f | L_TRIGGER : %f \n", triggerR, triggerL);

	//------------------------------------------------------------------------------+
	// �O�i
	//------------------------------------------------------------------------------+
	if (triggerR != 0.0f)
	{
		// �A�N�Z����Ԃɂ���
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_ACCEL);

		// �A�N�Z���̍ő�l�������Ă��Ȃ���΍X�V���A�����Ă�����ő�l�𐳋K��
		if (m_accelValue < m_accelLimit)
		{
			//m_accelValue += speed * (triggerR * in_deltaTime);
			m_accelValue += speed * triggerR;
		}
		else
		{
			m_accelValue = m_accelLimit;
		}
	}
	else if (m_accelValue != 0.0f)       // �A�N�Z�����|�����Ă��鎞���X�V
	{
		m_accelValue -= (m_accelValue / 1.5f) * in_deltaTime;
	}

	//---------------------------------------------------------------------------------+
	// ���
	//---------------------------------------------------------------------------------+
	if (triggerL != 0.0f)
	{
		// �ړ���Ԃ��u���[�L�ɂ���
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_BRAKE);

		// �u���[�L�̍ő�l�������Ă��Ȃ���΍X�V���A�����Ă���΍ő�l�𐳋K��
		if (m_brakeValue < m_brakeLimit)
		{
			//m_brakeValue += speed * (triggerL * in_deltaTime);
			m_brakeValue += speed * triggerL;
		}
		else
		{
			m_brakeValue = m_brakeLimit;
		}
	}
	else if (m_brakeValue != 0.0f)
	{
		m_brakeValue -= (m_brakeValue / 1.5f) * in_deltaTime;
	}

	//-----------------------------------------------------------------------------------+
	// �����x��0�ɖ߂�����
	//-----------------------------------------------------------------------------------+
	// �A�N�Z���E�u���[�L�����l�ȉ��Ȃ�0�ɖ߂�
	if (m_accelValue < 0.2f)
	{
		m_accelValue = 0.0f;
	}
	if (m_brakeValue < 0.2f)
	{
		m_brakeValue = 0.0f;
	}
	// �A�N�Z���E�u���[�L�����Ƃ�0�Ȃ�IDLE��Ԃ�
	if (m_accelValue == 0.0f && m_brakeValue == 0.0f)
	{
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_IDLE);
	}

	//----------------------------------------------------------------------------------+
	// �A�N�Z���E�u���[�L�̑��E����
	//----------------------------------------------------------------------------------+
	// �A�N�Z���̒l�����ȉ����A�N�Z���ƃu���[�L�������͂ő��E
	if (triggerR == 1.0f && triggerL == 1.0f)
	{
		if (m_accelValue >= m_brakeValue)
		{
			m_accelValue -= m_accelValue / 10.0f;
		}
		else
		{
			m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_IDLE);
			m_accelValue = m_brakeValue;
		}
	}

	// �A�N�Z���E�o�b�N�̑��x��\�� (�f�o�b�O)
	// printf("ACCEL_SPEED : %f | BACK_SPEED : %f \n", m_accel, m_brake);

	// �n���h�u���[�L����(�R���g���[���[A�{�^�������Ŕ���)
	if (CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A) && m_accelValue > 5.0f)
	{
		m_handBrake = HAND_BRAKE_VALUE;
		m_accelValue -= m_handBrake;
	}
	else
	{
		m_handBrake = 0;
	}

	// �O�i�l���X�V
	DirVec.x += forwardVec.x * (m_accelValue + -m_brakeValue) * in_deltaTime;

	// ���E�̉�]�A�ړ�(�A�N�Z�����O��ǂ��炩�ɂ������Ă��鎞)
	if (m_accelValue >= 5.0f || m_brakeValue >= 5.0f)
	{
		// �A�N�Z����
		if (m_playerCar->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_ACCEL)
		{
			// ���E��]
			DirVec.y += rightVec.y * axisL.x * (m_accelValue / m_accelLimit) * (m_accelLimit / 5.0f) * in_deltaTime;
			// Z����]
			//m_radian += axisL.x * (m_accelValue / m_accelLimit) * (m_accelLimit / 150.0f) * in_deltaTime;
			m_radian += axisL.x * (m_accelValue / m_accelLimit) * 0.5f * in_deltaTime;

		}
		// �o�b�N��
		if (m_playerCar->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_BRAKE)
		{
			// ���E��]
			DirVec.y -= rightVec.y * -axisL.x * (m_brakeValue / m_brakeLimit) * 0.5f * in_deltaTime;
			// Z����]
			m_radian -= -axisL.x * (m_brakeValue / m_brakeLimit) * 0.5f * in_deltaTime;
		}

		// ���E��
		if (m_accelValue >= 1.0f && m_brakeValue >= 1.0f)
		{
			// ���E��]
			DirVec.y += rightVec.y * axisL.x * 0.5f * in_deltaTime;
			// Z����]
			m_radian += axisL.x * 0.5f * in_deltaTime;
		}

	}


	// �O�i�܂��͌�ގ��̂ݑO�i�x�N�g�����X�V
	if (DirVec.x > 0.5f || DirVec.x < -0.5f)
	{
		// �����L�[���͂̒l��O�i�x�N�g���ɃZ�b�g
		charaForwardVec = DirVec;
	}
	else
	{
		// �O�i�x�N�g�����X�V�����Ȃ�
		charaForwardVec = Vector3(0, 0, 0);
	}

	// �O�t���[���̃��W�A���ƌ��݂̃��W�A�����r���A�v���C���[���ǂ���ɐ��񂵂悤�Ƃ��Ă��邩���X�V
	if (m_radian < m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_LEFT);
	}
	if (m_radian > m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_RIGHT);
	}
	if (m_radian == m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_IDLE);
	}
	// ���W�A�����X�V
	m_pastRadian = m_radian;


	// ���݂̃X�s�[�h��ۑ�
	m_forwardSpeed = speed * in_deltaTime;

	// Z���̉�]���X�V
	Quaternion rotation = Quaternion::Quaternion(Vector3::UnitZ, m_radian);
	m_owner->SetRotation(rotation);
	rotation = m_owner->GetRotation();

	// Rotation����O�i�x�N�g�����X�V���A���ʂ̍��W���Z�o
	charaForwardVec = Vector3::Transform(charaForwardVec, rotation);
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += m_forwardSpeed * charaForwardVec;

	// �I�[�i�[�֓n��
	m_owner->SetPosition(resultPos);
}

void MoveComponentCar::MovementByKeyboard(float in_deltaTime)
{

	//�L��������
	const float speed = 5.0f;
	Vector3 forwardVec = Vector3(60.0f, 0.0f, 0.0f);
	Vector3 rightVec = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = m_owner->GetForward();

	// �L�����N�^�[�ړ�
	Vector3 DirVec(0.0f, 0.0f, 0.0f);

	//------------------------------------------------------------------------------------------------+
	// �L�[�{�[�h����
	//------------------------------------------------------------------------------------------------+
	// �E�g���K�[�A���g���K�[�擾
	float triggerR = 0.0f;
	float triggerL = 0.0f;
	// ���X�e�B�b�N���͒l���擾
	float vertAxis = 0.0f;

	// �L�[���͎擾(�A�N�Z��)
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_UP))
	{
		triggerR = 1.0f;
	}
	else
	{
		triggerR = 0.0f;
	}

	// �L�[���͎擾(�o�b�N)
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_DOWN))
	{
		triggerL = 1.0f;
	}
	else
	{
		triggerL = 0.0f;
	}

	// �L�[���͎擾(���E�ړ�)
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LEFT))
	{
		vertAxis = -1.0f;
	}
	else if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_RIGHT))
	{
		vertAxis = 1.0f;
	}
	else
	{
		vertAxis = 0.0f;
	}

	// �g���K�[���͒l��\��
	// printf("R_TRIGGER : %f | L_TRIGGER : %f \n", triggerR, triggerL);

	//------------------------------------------------------------------------------+
	// �O�i
	//------------------------------------------------------------------------------+
	if (triggerR != 0.0f)
	{
		// �A�N�Z����Ԃɂ���
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_ACCEL);

		// �A�N�Z���̍ő�l�������Ă��Ȃ���΍X�V���A�����Ă�����ő�l�𐳋K��
		if (m_accelValue < m_accelLimit)
		{
			m_accelValue += speed * (triggerR * in_deltaTime);

		}
		else
		{
			m_accelValue = m_accelLimit;
		}
	}
	else if (m_accelValue != 0.0f)       // �A�N�Z�����|�����Ă��鎞���X�V
	{
		m_accelValue -= (m_accelValue / 1.5f) * in_deltaTime;
	}

	//---------------------------------------------------------------------------------+
	// ���
	//---------------------------------------------------------------------------------+
	if (triggerL != 0.0f)
	{
		// �A�N�Z����Ԃɂ���
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_BRAKE);

		// �u���[�L�̍ő�l�������Ă��Ȃ���΍X�V���A�����Ă���΍ő�l�𐳋K��
		if (m_brakeValue < m_brakeLimit)
		{
			m_brakeValue += 8.0f * (triggerL * in_deltaTime);
		}
		else
		{
			m_brakeValue = m_brakeLimit;
		}

	}
	else if (m_brakeValue != 0.0f)
	{
		m_brakeValue -= (m_brakeValue / 1.5f) * in_deltaTime;
	}


	//-----------------------------------------------------------------------------------+
	// �����x��0�ɖ߂�����
	//-----------------------------------------------------------------------------------+
	// �A�N�Z���E�u���[�L�����l�ȉ��Ȃ�0�ɖ߂�
	if (m_accelValue < 0.2f)
	{
		m_accelValue = 0.0f;
	}
	if (m_brakeValue < 0.2f)
	{
		m_brakeValue = 0.0f;
	}
	// �A�N�Z���E�u���[�L�����Ƃ�0�Ȃ�IDLE��Ԃ�
	if (m_accelValue == 0.0f && m_brakeValue == 0.0f)
	{
		m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_IDLE);
	}

	//----------------------------------------------------------------------------------+
	// �A�N�Z���E�u���[�L�̑��E����
	//----------------------------------------------------------------------------------+
	// �A�N�Z���̒l�����ȉ����A�N�Z���ƃu���[�L�������͂ő��E
	if (triggerR == 1.0f && triggerL == 1.0f)
	{
		if (m_accelValue >= m_brakeValue)
		{
			m_accelValue -= m_accelValue / 10.0f;
		}
		else
		{
			m_playerCar->SetDriveState(PlayerCar::DRIVE_STATE::DRIVE_IDLE);
			m_accelValue = m_brakeValue;
		}
	}

	// �A�N�Z���E�o�b�N�̑��x��\�� (�f�o�b�O)
	// printf("ACCEL_SPEED : %f | BACK_SPEED : %f \n", m_accel, m_brake);

	// �O�i�l���X�V
	DirVec.x += forwardVec.x * (m_accelValue + -m_brakeValue) * in_deltaTime;


	// ���E�̉�]�A�ړ�(�A�N�Z�����O��ǂ��炩�ɂ������Ă��鎞)
	if (m_accelValue != 0.0f || m_brakeValue != 0.0f)
	{
		// �A�N�Z����
		if (m_playerCar->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_ACCEL)
		{
			// ���E��]
			DirVec.y += rightVec.y * vertAxis * (m_accelValue / m_accelLimit) * (m_accelLimit / 5.0f) * in_deltaTime;
			// Z����]
			m_radian += vertAxis * (m_accelValue / m_accelLimit) * (m_accelLimit / 60.0f) * in_deltaTime;
		}
		// �o�b�N��
		if (m_playerCar->GetDriveState() == PlayerCar::DRIVE_STATE::DRIVE_BRAKE)
		{
			// ���E��]
			DirVec.y += rightVec.y * vertAxis * (m_brakeValue / m_brakeLimit) * 0.5f * in_deltaTime;
			// Z����]
			m_radian += vertAxis * (m_brakeValue / m_brakeLimit) * 0.5f * in_deltaTime;
		}

		// ���E��
		if (m_accelValue >= 1.0f && m_brakeValue >= 1.0f)
		{
			// ���E��]
			DirVec.y += rightVec.y * vertAxis * 0.5f * in_deltaTime;
			// Z����]
			m_radian += vertAxis * 0.5f * in_deltaTime;
		}

	}

	// �O�t���[���̃��W�A���ƌ��݂̃��W�A�����r���A�v���C���[���ǂ���ɐ��񂵂悤�Ƃ��Ă��邩���X�V
	if (m_radian < m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_LEFT);
	}
	if (m_radian > m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_RIGHT);
	}
	if (m_radian == m_pastRadian)
	{
		m_playerCar->SetTurnState(PlayerCar::TURNING_STATE::TURN_IDLE);
	}
	// ���W�A�����X�V
	m_pastRadian = m_radian;


	// �O�i�܂��͌�ގ��̂ݑO�i�x�N�g�����X�V
	if (DirVec.x > 0.5f || DirVec.x < -0.5f)
	{
		// �����L�[���͂̒l��O�i�x�N�g���ɃZ�b�g
		charaForwardVec = DirVec;

	}
	else
	{

		charaForwardVec = Vector3(0, 0, 0);
	}

	// ���݂̃X�s�[�h��ۑ�
	m_forwardSpeed = speed * in_deltaTime;

	// Z���̉�]���X�V
	Quaternion rotation = Quaternion::Quaternion(Vector3::UnitZ, m_radian);
	m_owner->SetRotation(rotation);
	rotation = m_owner->GetRotation();

	// Rotation����O�i�x�N�g�����X�V���A���ʂ̍��W���Z�o
	charaForwardVec = Vector3::Transform(charaForwardVec, rotation);
	Vector3 resultPos = m_owner->GetPosition();
	resultPos += m_forwardSpeed * charaForwardVec * m_playerCar->GetFrictionVal();

	// �I�[�i�[�֓n��
	m_owner->SetPosition(resultPos);

}
