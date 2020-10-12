#include "HumanCamera.h"
#include "PlayerHuman.h"

const float HumanCamera::CAMERA_SENSITIVITY = 20.0f;    // �J�������x(�Ⴂ�قǍ����x)
const Vector3 HumanCamera::DEFAULT_DISTANCE_OFFSET = Vector3(-50.0f, 0.0f, 20.0f);
const float HumanCamera::MIN_TARGET_DISTANCE = -10.0f;
const float HumanCamera::MAX_TARGET_DISTANCE = 150.0f;

HumanCamera::HumanCamera(PlayerHuman* in_owner)
	:CameraComponent(in_owner)
	,m_player(in_owner)
	, m_offset(DEFAULT_DISTANCE_OFFSET)
	, m_upVec(Vector3::UnitZ)
	,m_playerForward(Vector3::Zero)
	, m_yaw(0.0f)
	,m_pitch(0.0f)
	,m_pitchSpeed(0.0f)
	,m_maxPitch(Math::Pi / 3.0f)
	, m_distance(-30.0f)
	, m_mousePos(MOUSE_INSTANCE.GetPosition())
	, m_frameMousePos(MOUSE_INSTANCE.GetPosition())
{

}

HumanCamera::~HumanCamera()
{
}

// �X�V����
void HumanCamera::Update(float in_deltaTime)
{

	// ���͏���
	ProcessInput(in_deltaTime);

	// �I�[�i�[�A�N�^�̍��W���擾
	Vector3 targetPos = m_owner->GetPosition();

	Matrix4 view;

	//----------------------------------------------------------------------------------+
    //
    // ��l�̎��_���̏���
    //
    //----------------------------------------------------------------------------------+
	if (m_player->GetPlayerPOV() == PlayerHuman::POV_FIRST_PERSON)
	{
		// �s�b�`���I�[�i�[���ōX�V���ꂽ���x����X�V
		m_pitch += m_pitchSpeed * in_deltaTime;
		// �ő�E�ŏ��l�Ƀs�b�`���N�����v
		m_pitch = Math::Clamp(m_pitch, -m_maxPitch, m_maxPitch);
		// �s�b�`����Quaternion���쐬
		Quaternion pitchRot = Quaternion::Quaternion(m_owner->GetRight(), m_pitch);

		// �I�[�i�[�A�N�^�̑O�i�x�N�g���ƃs�b�`��Quaternion����A�J�����̑O�i�x�N�g�����Z�o
		Vector3 viewForward = Vector3::Transform(m_owner->GetForward(), pitchRot);

		// 
		Vector3 target = targetPos + viewForward * 100.0f;
		// ������x�N�g�����쐬
		Vector3 upVec = Vector3::Transform(Vector3::UnitZ, pitchRot);

		// �r���[�s����쐬���A�����_���[�փZ�b�g
		view = Matrix4::CreateLookAt(targetPos, target, upVec);

		// �v���C���[�̑O�i�x�N�g���X�V
		m_player->RotateToNewForward(viewForward);

	}

	//----------------------------------------------------------------------------------+
	//
	// �O�l�̎��_���̏���
	//
	//----------------------------------------------------------------------------------+
	if (m_player->GetPlayerPOV() == PlayerHuman::POV_THIRD_PERSON)
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
		printf("position.x : %f, position.y : %f, position.z : %f\n", m_position.x, m_position.y, m_position.z);
		// �r���[�s����X�V
		view = Matrix4::CreateLookAt(m_position, targetPos, m_upVec);

		// �����x�N�g��
		Vector3 dist = Vector3(-15.0f, -35.0f, m_distance);
		// �����������Z
		view = view * Matrix4::CreateTranslation(dist);



	}

	// �v���C���[�̑O�i�x�N�g���X�V
	m_player->RotateToNewForward(m_playerForward);

	// �����_���[�̃r���[�s��X�V
	SetViewMatrix(view);
}


// ���͏���
void HumanCamera::ProcessInput(float in_deltaTime)
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

			// �v���C���[�̑O�i�x�N�g�����X�V
			m_playerForward = m_player->GetPosition() - m_position;
			m_playerForward.z = 0.0f;
			m_playerForward.Normalize();

		}

		// ���[�v�Z
		yawSpeed = axisR.x / CAMERA_SENSITIVITY;
		yawSpeed *= maxOrbitSpeed;


		// �s�b�`�̍ő�E�ŏ��p�x�𒲐�
		const float pitchMaxDegree = 40.0f; // �J�����s�b�`�ō��p�x(degree)
		const float pitchMinDegree = -20.0f; // �J�����s�b�`�Œ�p�x(degree)
		if (axisR.y > 0.0f && m_offset.z + pitchSpeed > pitchMaxDegree)
		{
			pitchSpeed = 0.0f;
		}
		if (axisR.y < 0.0f && m_offset.z + pitchSpeed < pitchMinDegree)
		{
			pitchSpeed = 0.0f;		
		}


		// ���[�������o�ɃZ�b�g
		SetYaw(yawSpeed);
		// �s�b�`�������o�ɃZ�b�g
		SetPitch(pitchSpeed);

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


