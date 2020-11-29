#include "ThirdPersonCamera.h"
#include "Actor.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "MeshComponent.h"

const float ThirdPersonCamera::CAMERA_SENSITIVITY = 20.0f;    // �J�������x(�Ⴂ�قǍ����x)
const Vector3 ThirdPersonCamera::DEFAULT_DISTANCE_OFFSET = Vector3(-50.0f, 0.0f, 20.0f);
const float ThirdPersonCamera::MIN_TARGET_DISTANCE = 0.0f;
const float ThirdPersonCamera::MAX_TARGET_DISTANCE = 200.0f;

ThirdPersonCamera::ThirdPersonCamera(Actor* in_owner)
	:CameraComponent(in_owner)
	, m_offset(Vector3(50.0f, -30.0f, 10.0f))
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

	// �����x�N�g��������
	m_forwardVec = -1.0f * m_offset;
	m_forwardVec.Normalize();


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

	float height = -35.0f;      // �J�����̍���


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
		Vector3 dist = Vector3(-15.0f, height, m_distance);
		// �����������Z
		view = view * Matrix4::CreateTranslation(dist);

		
		m_cameraActor->SetPosition(m_position);
		m_cameraActor->ComputeWorldTransform();
		m_hitBox->OnUpdateWorldTransform();
	}

	//printf("Camera : x : %f, y : %f, z : %f\n", m_position.x, m_position.y, m_position.z);
	//printf("hitBox : x : %f, y : %f, z : %f\n", m_cameraActor->GetPosition().x, m_cameraActor->GetPosition().y, m_cameraActor->GetPosition().z);

	// �����_���[�̃r���[�s��X�V
	SetViewMatrix(view);
}

void ThirdPersonCamera::ProcessInput(float in_deltaTime)
{
	const float maxOrbitSpeed = Math::Pi * 8;

	// ���[���x�A�s�b�`���x
	float yawSpeed, pitchSpeed;
	yawSpeed = pitchSpeed = 0.0f;

	float attenRate = 3.0f;    // �����䗦

	// �s�b�`�̍ő�p�x�E�ŏ��p�x
	const float pitchMaxDegree = 45.0f; // �J�����s�b�`�ō��p�x(degree)
	const float pitchMinDegree = -10.0f; // �J�����s�b�`�Œ�p�x(degree)

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

		// back�{�^���������Ă��Ȃ���
		if (!CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			

			// �J���������������Ă���ꍇ�Ɏ����␳
			if (m_distance > MAX_TARGET_DISTANCE / 2)
			{
				m_distance = MAX_TARGET_DISTANCE / 2;
			}

			// �O�i�x�N�g���X�V�������Ă��鎞�̂�
			if (m_adjustForward)
			{
				// �v���C���[�̑O�i�x�N�g�����X�V
				//m_ownerForward = m_owner->GetPosition() - m_position;
				//m_ownerForward.z = 0.0f;
				//m_ownerForward.Normalize();
				// �v���C���[�̑O�i�x�N�g���X�V
				//m_owner->RotateToNewForward(m_ownerForward);
			}
		}

		// �^�[�Q�b�g�܂ł̋�����ύX (back�{�^���������Ă��鎞)
		if (CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_BACK))
		{
			// �������E�X�e�B�b�N�̓��͒l����Z�o
			m_distance += axisR.y * 500.0f * in_deltaTime;
			// �t���[�J�������̓s�b�`��ύX���Ȃ�
			axisR.y = 0.0f;

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

	}

	//-----------------------------------------------------+
    //
    // �R���g���[�����ڑ����̏��� (�}�E�X����)
    //
    //-----------------------------------------------------+
	else
	{
		//SDL_SetRelativeMouseMode(SDL_TRUE);

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





		// �^�[�Q�b�g�܂ł̋�����ύX (F�L�[�������Ă��鎞)
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_F))
		{
			// �������E�X�e�B�b�N�̓��͒l����Z�o
			m_distance += (m_frameMousePos.y - m_mousePos.y) / CAMERA_SENSITIVITY * in_deltaTime;
			// �t���[�J�������̓s�b�`��ύX���Ȃ�


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
			float yoffset = (m_frameMousePos.y - m_mousePos.y) / CAMERA_SENSITIVITY * maxOrbitSpeed * in_deltaTime;
			m_pitch = Math::Lerp(m_pitch, -yoffset, attenRate * in_deltaTime);

			// �O�i�x�N�g���X�V�������Ă��鎞�̂�
			if (m_adjustForward)
			{
				// �v���C���[�̑O�i�x�N�g�����X�V
			    //	m_ownerForward = m_owner->GetPosition() - m_position;
			    //	m_ownerForward.z = 0.0f;
		     	//	m_ownerForward.Normalize();
				// �v���C���[�̑O�i�x�N�g���X�V
			    //m_owner->RotateToNewForward(m_ownerForward);
			}
		}

		// ���[�v�Z
		yawSpeed -= xoffset;
		m_yaw = Math::Lerp(m_yaw, -xoffset, attenRate * in_deltaTime);

		// �s�b�`�̍ő�E�ŏ��p�x�𒲐�
		if (m_offset.z + m_pitch > pitchMaxDegree)
		{
			m_pitch = 0.0f;
		}
		if (m_offset.z + m_pitch < pitchMinDegree)
		{
			m_pitch = 0.0f;
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





}

// �����蔻�菈��
void ThirdPersonCamera::CollisionFix(BoxCollider* in_hitCameraBox, BoxCollider* in_hitBox)
{
	// �����������b�V�����\��
	//in_hitBox->GetOwner()->GetMeshComponent()->SetVisible(false);

	Vector3 fix;


	//�ǂƂԂ������Ƃ�
	AABB bgBox = in_hitBox->GetWorldBox();
	AABB playerBox = m_hitBox->GetWorldBox();

	// �߂荞�݂��C��
	CalcCollisionFixVec(playerBox, bgBox, fix);

	if (m_position.x != fix.x || m_position.y != fix.y || m_position.z != fix.z)
	{
		//printf("���������I\n");
	}

	// �␳�x�N�g�����߂�
	m_position += fix;
	m_cameraActor->SetPosition(m_position);
	m_cameraActor->ComputeWorldTransform();


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
