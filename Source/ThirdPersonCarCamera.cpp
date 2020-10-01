#include "ThirdPersonCarCamera.h"
#include "Actor.h"
#include "PlayerCar.h"
#include "Input.h"
#include "InputController.h"
#include "Mouse.h"

const float ThirdPersonCarCamera::CAMERA_SENSITIVITY = 50.0f;

ThirdPersonCarCamera::ThirdPersonCarCamera(PlayerCar* in_target)
	:CameraComponent(in_target)
	, m_offset(-250.0f, 0.0f, 100.0f)
	, m_upVec(Vector3::UnitZ)
	, m_pitch(0.0f)
	, m_yaw(0.0f)
	,m_mousePos(MOUSE_INSTANCE.GetPosition())
	,m_frameMousePos(MOUSE_INSTANCE.GetPosition())
{

}

ThirdPersonCarCamera::~ThirdPersonCarCamera()
{
}

void ThirdPersonCarCamera::Update(float in_deltaTime)
{
	ProcessInput(in_deltaTime);

	// ���[�̃N�H�[�^�j�I�������[���h�ϊ��s��̏���x�N�g�����琶��
	Quaternion yaw(Vector3::UnitZ, m_yaw * in_deltaTime);

	// �I�t�Z�b�g�Ƀ��[�̒l�����čX�V
	m_offset = Vector3::Transform(m_offset, yaw);
	// �J�����̏���x�N�g�������[����X�V
	m_upVec = Vector3::Transform(m_upVec, yaw);

	// �J�����̑O�i�x�N�g�����I�t�Z�b�g����Z�o
	Vector3 forwardVec = -1.0f * m_offset;
	forwardVec.Normalize();

	// �E�����x�N�g��������x�N�g���ƑO�i�x�N�g������Z�o
	Vector3 rightVec = Vector3::Cross(m_upVec, forwardVec);
	rightVec.Normalize();

	// �J�����̉E�����x�N�g������s�b�`�̃N�H�[�^�j�I���𐶐�
	Quaternion pitch(rightVec, m_pitch * in_deltaTime);

	// �I�t�Z�b�g�Ƀs�b�`�̒l�����čX�V
	m_offset = Vector3::Transform(m_offset, pitch);
	// �J�����̏��x�N�g�����s�b�`����X�V
	m_upVec = Vector3::Transform(m_upVec, pitch);

	// ���[���h���W���^�[�Q�b�g�̍��W�ƃI�t�Z�b�g����Z�o
	Vector3 targetPos = m_owner->GetPosition();
	Vector3 cameraPos = targetPos + m_offset;

	// �r���[�s����X�V
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, targetPos, m_upVec);
	SetViewMatrix(view);
}

// �J�������͏���
void ThirdPersonCarCamera::ProcessInput(float in_deltaTime)
{

	const float maxOrbitSpeed = Math::Pi * 8;

	// �R���g���[���ڑ����Ɩ��ڑ����ŏ����𕪊�
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// �E�X�e�B�b�N�̓��͒l���i�[����ϐ�
		Vector2 axisR;
		axisR = CONTROLLER_INSTANCE.GetRAxisVec();


		// ���[���x�A�s�b�`���x
		float yawSpeed, pitchSpeed;
		yawSpeed = pitchSpeed = 0.0f;
		// ���[�v�Z
		yawSpeed = axisR.x / CAMERA_SENSITIVITY;
		yawSpeed *= maxOrbitSpeed;
		// �����o�ɃZ�b�g
		SetYaw(yawSpeed);

		// �s�b�`�v�Z
		pitchSpeed = axisR.y / CAMERA_SENSITIVITY;
		pitchSpeed *= maxOrbitSpeed;
		// �����o�ɃZ�b�g
		SetPitch(pitchSpeed);


	}
	else
	{

		// �}�E�X�̈ʒu����J�����̃��[�ƃs�b�`���擾
		m_mousePos = MOUSE_INSTANCE.GetPosition();
		//float xoffset = (m_frameMousePos.x - m_mousePos.x) / CAMERA_SENSITIVITY;
		float yoffset = (m_frameMousePos.y - m_mousePos.y) / CAMERA_SENSITIVITY;

		// x���W�̃I�t�Z�b�g
		float xoffset = 0.0f;
		// �}�E�X��x���W���O�t���[����x���W�Ɣ�ׂĒl���قȂ�ꍇ�A���[���X�V
		// ��ʒ���
		if (m_mousePos.x > m_frameMousePos.x || m_mousePos.x > m_frameMousePos.x && (m_mousePos.x - GAME_CONFIG->GetScreenWidth()) > 0.0f)
		{
			xoffset = -1.0f * in_deltaTime;
			xoffset *= maxOrbitSpeed;
		}
		else if (m_mousePos.x < m_frameMousePos.x || m_mousePos.x < m_frameMousePos.x && m_mousePos.x < GAME_CONFIG->GetScreenWidth())
		{
			xoffset = 1.0f * in_deltaTime;
			xoffset *= maxOrbitSpeed;
		}


		// ���[���x�A�s�b�`���x
		float yawSpeed, pitchSpeed;
		yawSpeed = pitchSpeed = 0.0f;

		// ���[�v�Z
		yawSpeed -= xoffset;
		//yawSpeed *= maxOrbitSpeed;
		// �����o�ɃZ�b�g
		SetYaw(yawSpeed);

		// �s�b�`�v�Z
		pitchSpeed -= yoffset;
		//pitchSpeed *= maxOrbitSpeed;
		// �����o�ɃZ�b�g
		SetPitch(pitchSpeed);

		if (m_mousePos.x != 1919.0f && m_mousePos.x > 0.0f)
		{
			m_frameMousePos.x = MOUSE_INSTANCE.GetPositionX();
		}
		if (m_mousePos.y != 1079.0f && m_mousePos.y != 0.0f)
		{
			m_frameMousePos.y = MOUSE_INSTANCE.GetPositionY();
		}
	}
}
