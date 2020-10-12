#include "ThirdPersonCamera.h"
#include "Actor.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"

const float ThirdPersonCamera::CAMERA_SENSITIVITY = 20.0f;    // �J�������x(�Ⴂ�قǍ����x)
const Vector3 ThirdPersonCamera::DEFAULT_DISTANCE_OFFSET = Vector3(-50.0f, 0.0f, 20.0f);
const float ThirdPersonCamera::MIN_TARGET_DISTANCE = 0.0f;
const float ThirdPersonCamera::MAX_TARGET_DISTANCE = 200.0f;

ThirdPersonCamera::ThirdPersonCamera(Actor* in_owner)
	:CameraComponent(in_owner)
	, m_offset(DEFAULT_DISTANCE_OFFSET)
	, m_upVec(Vector3::UnitZ)
	, m_ownerForward(m_owner->GetForward())
	, m_yaw(0.0f)
	, m_pitch(0.0f)
	, m_pitchSpeed(0.0f)
	, m_maxPitch(Math::Pi / 3.0f)
	, m_distance(20.0f)
	, m_adjustForward(true)
	, m_chaseOwnerForward(false)
	, m_mousePos(MOUSE_INSTANCE.GetPosition())
	, m_frameMousePos(MOUSE_INSTANCE.GetPosition())
{
	m_cameraActor = new Actor();
	m_cameraActor->SetPosition(m_position);


	// �����蔻��{�b�N�X�̃Z�b�g
	AABB hitBox;
	hitBox.m_min.x = -5.0f;
	hitBox.m_min.y = -5.0f;
	hitBox.m_min.z = -5.0f;
	hitBox.m_max.x = 5.0f;
	hitBox.m_max.y = 5.0f;
	hitBox.m_max.z = 5.0f;
	m_hitBox = new BoxCollider(m_cameraActor, PhysicsWorld::TYPE_CAMERA);
	m_hitBox->SetObjectBox(hitBox);
	m_hitBox->SetCamera(this);

}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Update(float in_deltaTime)
{

	// ���͏���
	ProcessInput(in_deltaTime);

	// �I�[�i�[�A�N�^�̍��W���擾
	Vector3 targetPos = m_owner->GetPosition();
	// �r���[�s��
	Matrix4 view;

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
		Vector3 forwardVec = -1.0f * m_offset;
		forwardVec.Normalize();

		// �E�����x�N�g��������x�N�g���ƑO�i�x�N�g������Z�o
		Vector3 rightVec = Vector3::Cross(m_upVec, forwardVec);
		rightVec.Normalize();


		// �J�����̉E�����x�N�g������s�b�`�̃N�H�[�^�j�I���𐶐�
		Quaternion pitch(rightVec, m_pitch * in_deltaTime);

		// �J�����̏��x�N�g�����s�b�`����X�V
		m_upVec = Vector3::Transform(m_upVec, pitch);

		// �I�t�Z�b�g�Ƀs�b�`�̒l�����čX�V
		m_offset = Vector3::Transform(m_offset, pitch);


		// ���[���h���W���^�[�Q�b�g�̍��W�ƃI�t�Z�b�g����Z�o
		//m_position = Vector3::Lerp(m_position, targetPos + m_offset, 2.0f * in_deltaTime);
		m_position = targetPos + m_offset;
		//printf("position.x : %f, position.y : %f, position.z : %f\n", m_position.x, m_position.y, m_position.z);
		// �r���[�s����X�V
		view = Matrix4::CreateLookAt(m_position, targetPos, m_upVec);

		// �����x�N�g��
		Vector3 dist = Vector3(-15.0f, -35.0f, m_distance);
		// �����������Z
		view = view * Matrix4::CreateTranslation(dist);


	}

	m_cameraActor->SetPosition(m_position);
	m_hitBox->OnUpdateWorldTransform();

	// �����_���[�̃r���[�s��X�V
	SetViewMatrix(view);
}

void ThirdPersonCamera::ProcessInput(float in_deltaTime)
{
	const float maxOrbitSpeed = Math::Pi * 8;

	// ���[���x�A�s�b�`���x
	float yawSpeed, pitchSpeed;
	yawSpeed = pitchSpeed = 0.0f;

	// �s�b�`�̍ő�p�x�E�ŏ��p�x
	const float pitchMaxDegree = 40.0f; // �J�����s�b�`�ō��p�x(degree)
	const float pitchMinDegree = -30.0f; // �J�����s�b�`�Œ�p�x(degree)

	//-----------------------------------------------------+
	//
	// �R���g���[���ڑ����̏���
	//
	//-----------------------------------------------------+
	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// �E�X�e�B�b�N�̓��͒l���i�[����ϐ�
		Vector2 axisR;
		axisR = CONTROLLER_INSTANCE.GetRAxisVec();


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
		if (!CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			// �J���������������Ă���ꍇ�Ɏ����␳
			if (m_distance > MAX_TARGET_DISTANCE / 2)
			{
				m_distance = MAX_TARGET_DISTANCE / 2;
			}

			// �s�b�`�v�Z
			pitchSpeed = axisR.y / CAMERA_SENSITIVITY;
			pitchSpeed *= maxOrbitSpeed;

			// �O�i�x�N�g���X�V�������Ă��鎞�̂�
			if (m_adjustForward)
			{
				// �v���C���[�̑O�i�x�N�g�����X�V
				m_ownerForward = m_owner->GetPosition() - m_position;
				m_ownerForward.z = 0.0f;
				m_ownerForward.Normalize();
				// �v���C���[�̑O�i�x�N�g���X�V
				m_owner->RotateToNewForward(m_ownerForward);
			}
		}



		// ���[�v�Z
		yawSpeed = axisR.x / CAMERA_SENSITIVITY;
		yawSpeed *= maxOrbitSpeed;



		// �s�b�`�̍ő�E�ŏ��p�x�𒲐�
		if (axisR.y > 0.0f && m_offset.z + pitchSpeed > pitchMaxDegree)
		{
			pitchSpeed = 0.0f;
		}
		if (axisR.y < 0.0f && m_offset.z + pitchSpeed < pitchMinDegree)
		{
			pitchSpeed = 0.0f;
		}

	}

	//-----------------------------------------------------+
    //
    // �R���g���[�����ڑ����̏��� (�}�E�X����)
    //
    //-----------------------------------------------------+
	else
	{

		// �}�E�X�̈ʒu����J�����̃��[�ƃs�b�`���擾
		m_mousePos = MOUSE_INSTANCE.GetPosition();


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



		// ���[�v�Z
		yawSpeed -= xoffset;
		//yawSpeed *= maxOrbitSpeed;


		//pitchSpeed *= maxOrbitSpeed;

		// �^�[�Q�b�g�܂ł̋�����ύX (F�L�[�������Ă��鎞)
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_F))
		{
			// �������E�X�e�B�b�N�̓��͒l����Z�o
			m_distance += (m_frameMousePos.y - m_mousePos.y) / CAMERA_SENSITIVITY * in_deltaTime;

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

		// F�L�[�������Ă��Ȃ���
		if (!INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_F))
		{
			// �J���������������Ă���ꍇ�Ɏ����␳
			if (m_distance > MAX_TARGET_DISTANCE / 2)
			{
				m_distance = MAX_TARGET_DISTANCE / 2;
			}

			// �s�b�`�v�Z
			float yoffset = (m_frameMousePos.y - m_mousePos.y) / CAMERA_SENSITIVITY;
			pitchSpeed -= yoffset;

			// �O�i�x�N�g���X�V�������Ă��鎞�̂�
			if (m_adjustForward)
			{
				// �v���C���[�̑O�i�x�N�g�����X�V
				m_ownerForward = m_owner->GetPosition() - m_position;
				m_ownerForward.z = 0.0f;
				m_ownerForward.Normalize();
				// �v���C���[�̑O�i�x�N�g���X�V
				m_owner->RotateToNewForward(m_ownerForward);
			}
		}


		// �s�b�`�̍ő�E�ŏ��p�x�𒲐�
		if (m_mousePos.y > 0.0f && m_offset.z + pitchSpeed > pitchMaxDegree)
		{
			pitchSpeed = 0.0f;
		}
		if (m_mousePos.y < 0.0f && m_offset.z + pitchSpeed < pitchMinDegree)
		{
			pitchSpeed = 0.0f;
		}

		// �O�t���[���̃}�E�X���W���X�V
		if (m_mousePos.x != 1919.0f && m_mousePos.x > 0.0f)
		{
			m_frameMousePos.x = m_mousePos.x;
		}
		if (m_mousePos.y != 1079.0f && m_mousePos.y != 0.0f)
		{
			m_frameMousePos.y = m_mousePos.y;
		}
	}



	// ���[�ƃs�b�`�������o�ɃZ�b�g
	m_yaw = yawSpeed;
	m_pitch = pitchSpeed;;


}

// �����蔻�菈��
void ThirdPersonCamera::CollisionFix(BoxCollider* in_hitCameraBox, BoxCollider* in_hitBox)
{
	// �C���p�x�N�g��
	Vector3 fix = Vector3::Zero;

	//�ǂƂԂ������Ƃ�
	AABB bgBox = in_hitBox->GetWorldBox();
	AABB cameraBox = m_hitBox->GetWorldBox();

	fix *= 2;

	// �߂荞�݂��C��
	CalcCollisionFixVec(cameraBox, bgBox, fix);

	// �␳�x�N�g�����߂�
	m_position += fix;

	m_cameraActor->SetPosition(m_position);

	// �r���[�s����X�V
	Matrix4 view = Matrix4::CreateLookAt(m_position, m_owner->GetPosition() + m_offset, m_upVec);

	// �����_���[�̃r���[�s��X�V
	SetViewMatrix(view);



	if (fix.x != 0.0f || fix.y != 0.0f || fix.z != 0.0f)
	{
		printf("���������I\n");
	}


	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	m_hitBox->OnUpdateWorldTransform();
}

// �J���������̃Z�b�^�[ (�ő�E�ŏ��l�𒴂��Ă��܂����ꍇ�ɂ͒�����������)
void ThirdPersonCamera::SetDistance(float in_dist)
{

	m_distance = in_dist;

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

// �I�[�i�[�̌�������苗���ɃJ�����ʒu�𒲐����A���̍��W��Ԃ�
const Vector3& ThirdPersonCamera::ComputeCameraPos() const
{
	// �I�[�i�[�A�N�^�[�ւ̍��W���擾
	Vector3 cameraPos = m_owner->GetPosition();

	// �O�i�x�N�g���̎擾
	Vector3 forwardVec = m_owner->GetForward();

	// �O�i�x�N�g���̔��Α��ɁA���̋����E������ۂ�����ԂɍX�V
	forwardVec = (DEFAULT_DISTANCE_OFFSET.x - 100.0f) * forwardVec;
	forwardVec.z = DEFAULT_DISTANCE_OFFSET.z + 50.0f;

	// ���W�̍X�V
	cameraPos += forwardVec;

	return cameraPos;
}
