#include "OrbitCamera.h"
#include "Actor.h"
#include "Input.h"
#include "InputController.h"

OrbitCamera::OrbitCamera(Actor* in_target)
	:CameraComponent(in_target)
	,m_offset(-250.0f, 0.0f, 100.0f)
	,m_upVec(Vector3::UnitZ)
	,m_pitch(0.0f)
	,m_yaw(0.0f)
{
	
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::Update(float in_deltaTime)
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

	// ���ۂ̃��[���h���W���^�[�Q�b�g�̍��W�ƃI�t�Z�b�g����Z�o
	Vector3 targetPos = m_owner->GetPosition();
	Vector3 cameraPos = targetPos + m_offset;

	// �r���[�s����X�V
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, targetPos, m_upVec);
	SetViewMatrix(view);
}

// �J�������͏���
void OrbitCamera::ProcessInput(float in_deltaTime)
{
	// �R���g���[���ڑ����Ɩ��ڑ����ŏ����𕪊�
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// �E�X�e�B�b�N�̓��͒l���i�[����ϐ�
		Vector2 axisR;
		axisR = CONTROLLER_INSTANCE.GetRAxisVec();

		const int maxCameraSpeed = 50;
		const float maxOrbitSpeed = Math::Pi * 8;

		// ���[���x�A�s�b�`���x
		float yawSpeed, pitchSpeed;
		yawSpeed = pitchSpeed = 0.0f;
		// ���[�v�Z
		yawSpeed = axisR.x / maxCameraSpeed;
		yawSpeed *= maxOrbitSpeed;
		// �����o�ɃZ�b�g
		SetYaw(yawSpeed);

		// �s�b�`�v�Z
		pitchSpeed = axisR.y / maxCameraSpeed;
		pitchSpeed *= maxOrbitSpeed;
		// �����o�ɃZ�b�g
		SetPitch(pitchSpeed);


	}
	else
	{




	}
}
