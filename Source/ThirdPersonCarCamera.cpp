#include "ThirdPersonCarCamera.h"
#include "Actor.h"
#include "PlayerCar.h"
#include "Input.h"
#include "InputController.h"
#include "Mouse.h"

const float ThirdPersonCarCamera::CAMERA_SENSITIVITY = 65.0f;
const Vector3 ThirdPersonCarCamera::DEFAULT_DISTANCE_OFFSET = Vector3(-150.0f, 0.0f, 70.0f);
const float ThirdPersonCarCamera::MIN_TARGET_DISTANCE = -80.0f;
const float ThirdPersonCarCamera::MAX_TARGET_DISTANCE = 300.0f;

ThirdPersonCarCamera::ThirdPersonCarCamera(PlayerCar* in_target)
	:CameraComponent(in_target)
	,m_playerCar(in_target)
	, m_offset(Vector3(150.0f, -30.0f, 50.0f))
	, m_upVec(Vector3::UnitZ)
	,m_velocity(Vector3::Zero)
	,m_pitch(0.0f)
	,m_yaw(0.0f)
	,m_distance(0.0f)
	,m_mousePos(MOUSE_INSTANCE.GetPosition())
	,m_frameMousePos(MOUSE_INSTANCE.GetPosition())
	,m_chaseOwnerForward(false)
{

}

ThirdPersonCarCamera::~ThirdPersonCarCamera()
{
}

void ThirdPersonCarCamera::Update(float in_deltaTime)
{

	// ���͏���
	ProcessInput(in_deltaTime);

	// �I�[�i�[�A�N�^�̍��W���擾
	Vector3 targetPos = m_owner->GetPosition();
	// �r���[�s��
	Matrix4 view;

	float height = -35.0f;      // �J�����̍���


	// �J�������w���Ǐ]���邩���Ȃ���
	if (CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT) != 0.0f)
	{
		m_chaseOwnerForward = true;
	}


	//----------------------------------------------------+
	// �J�����Ǐ]����
	//----------------------------------------------------+
	if (m_chaseOwnerForward)
	{
		// �΂˒萔
		const float springConstant = 128.0f;

		// �΂˒萔���猸�����v�Z
		float dampening = 3.0f * Math::Sqrt(springConstant);

		// ���z�̈ʒu���v�Z
		Vector3 idealPos = ComputeCameraPos();

		// ���ݒn�Ɨ��z�̍����v�Z
		Vector3 diff = m_position - idealPos;


		// �΂˂ɂ������x
		Vector3 accel = -springConstant * diff - dampening * m_velocity;

		// ���x�̍X�V
		m_velocity += accel * in_deltaTime;
		// ���ۂ̃J�����ʒu���X�V
		m_position += m_velocity * in_deltaTime;

		m_offset = idealPos - m_owner->GetPosition();
		m_upVec = Vector3::UnitZ;
		m_yaw = m_pitch = 0.0f;


		view = Matrix4::CreateLookAt(m_position, targetPos, m_upVec);
	}


	//---------------------------------------------------------+
	// �J�����Ǐ]����
	//---------------------------------------------------------+
	else
	{
		// ���[�̃N�H�[�^�j�I�������[���h�ϊ��s��̏���x�N�g�����琶��
		Quaternion yaw(Vector3::UnitZ, m_yaw * in_deltaTime);

		// �I�t�Z�b�g�Ƀ��[�̒l�����čX�V
		m_offset = Vector3::Transform(m_offset, yaw);
		// �J�����̏���x�N�g�������[����X�V
		m_upVec = Vector3::Transform(m_upVec, yaw);

		// �J�����̑O�i�x�N�g�����I�t�Z�b�g����Z�o
		m_forwardVec = -1.0f * m_offset;
		m_forwardVec.Normalize();

		// �E�����x�N�g��������x�N�g���ƑO�i�x�N�g������Z�o
		Vector3 rightVec = Vector3::Cross(m_upVec, m_forwardVec);
		rightVec.Normalize();


		// �J�����̉E�����x�N�g������s�b�`�̃N�H�[�^�j�I���𐶐�
		Quaternion pitch(rightVec, m_pitch * in_deltaTime);

		// �J�����̏��x�N�g�����s�b�`����X�V
		m_upVec = Vector3::Transform(m_upVec, pitch);

		// �I�t�Z�b�g�Ƀs�b�`�̒l�����čX�V
		m_offset = Vector3::Transform(m_offset, pitch);


		// ���[���h���W���^�[�Q�b�g�̍��W�ƃI�t�Z�b�g����Z�o
		//m_position = Vector3::Lerp(m_position, targetPos + m_offset, 2.0f * in_deltaTime);

		Vector3 pos = targetPos + m_offset;
		//m_position = Vector3::Lerp(m_position, pos, attenRate * in_deltaTime);
		m_position = targetPos + m_offset;
		//printf("position.x : %f, position.y : %f, position.z : %f\n", m_position.x, m_position.y, m_position.z);
		// �r���[�s����X�V
		view = Matrix4::CreateLookAt(m_position, targetPos, m_upVec);

		// �����x�N�g��
		Vector3 dist = Vector3(0.0f, 0.0, m_distance);
		// �����������Z
		view = view * Matrix4::CreateTranslation(dist);

	}

	// �����_���[�̃r���[�s��X�V
	SetViewMatrix(view);

}

// �J�������͏���
void ThirdPersonCarCamera::ProcessInput(float in_deltaTime)
{

	const float maxOrbitSpeed = Math::Pi * 8;
	float attenRate = 3.0f;    // �����䗦

	// �R���g���[���ڑ����Ɩ��ڑ����ŏ����𕪊�
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// �E�X�e�B�b�N�̓��͒l���i�[����ϐ�
		Vector2 axisR;
		axisR = CONTROLLER_INSTANCE.GetRAxisVec();

		
		if (axisR.x >= 0.9f || axisR.x <= -0.9f || axisR.y >= 0.9f || axisR.y <= -0.9f)
		{
			m_chaseOwnerForward = false;
		}

		// ���[���x�A�s�b�`���x
		float yawSpeed, pitchSpeed;
		yawSpeed = pitchSpeed = 0.0f;

		// �s�b�`�̍ő�p�x�E�ŏ��p�x
		const float pitchMaxDegree = 80.0f; // �J�����s�b�`�ō��p�x(degree)
		const float pitchMinDegree = 20.0f; // �J�����s�b�`�Œ�p�x(degree)

		// �^�[�Q�b�g�܂ł̋�����ύX (back�{�^���������Ă��鎞)
		if (CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			// �������E�X�e�B�b�N�̓��͒l����Z�o
			m_distance += axisR.y * 500.0f * in_deltaTime;

			// �ŏ������E�ő勗���̒���
			if (m_distance < MIN_TARGET_DISTANCE)
			{
				m_distance = MIN_TARGET_DISTANCE;
			}
			if (m_distance > MAX_TARGET_DISTANCE)
			{
				m_distance = MAX_TARGET_DISTANCE;
			}
		}

		// back�{�^���������Ă��Ȃ���
		if(!CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			// �J���������������Ă���ꍇ�Ɏ����␳
			if (m_distance > MAX_TARGET_DISTANCE / 2)
			{
				m_distance = MAX_TARGET_DISTANCE / 2;
			}


			// �s�b�`�v�Z
			pitchSpeed = axisR.y / CAMERA_SENSITIVITY;
			pitchSpeed *= maxOrbitSpeed;

		}

		// ���[�v�Z
		// �s�b�`�v�Z
		pitchSpeed = axisR.y / CAMERA_SENSITIVITY;
		pitchSpeed *= maxOrbitSpeed;
		m_pitch = Math::Lerp(m_pitch, (axisR.y / CAMERA_SENSITIVITY * maxOrbitSpeed), attenRate * in_deltaTime);
		// ���[�v�Z
		m_yaw = Math::Lerp(m_yaw, (axisR.x / CAMERA_SENSITIVITY * maxOrbitSpeed), attenRate * in_deltaTime);


		// �s�b�`�̍ő�E�ŏ��p�x�𒲐�
		if (axisR.y > 0.0f && m_offset.z + m_pitch > pitchMaxDegree)
		{
			m_pitch = 0.0f;
		}
		if (axisR.y < 0.0f && m_offset.z + m_pitch < pitchMinDegree)
		{
			m_pitch = 0.0f;
		}
			//printf("distance = %f\n", m_offset.x);



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
		//SetYaw(yawSpeed);

		// �s�b�`�v�Z
		pitchSpeed -= yoffset;
		//pitchSpeed *= maxOrbitSpeed;
		// �����o�ɃZ�b�g
		//SetPitch(pitchSpeed);

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


void ThirdPersonCarCamera::SetDistance(float in_dist)
{
}

// �I�[�i�[�̌�������苗���ɃJ���������킹��
const Vector3& ThirdPersonCarCamera::ComputeCameraPos() const
{
	// �I�[�i�[�A�N�^�[�ւ̍��W���擾
	Vector3 cameraPos = m_playerCar->GetPosition();

	// �O�i�x�N�g���̎擾
	Vector3 forwardVec = m_playerCar->GetForward();

	// �O�i�x�N�g���̔��Α��ɁA���̋����E������ۂ�����ԂɍX�V
	forwardVec = DEFAULT_DISTANCE_OFFSET.x * forwardVec;
	forwardVec.z = DEFAULT_DISTANCE_OFFSET.z;

	// ���W�̍X�V
	cameraPos += forwardVec;

	return cameraPos;
}
