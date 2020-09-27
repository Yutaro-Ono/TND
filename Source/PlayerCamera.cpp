#include "PlayerCamera.h"

// �R���X�g���N�^
PlayerCamera::PlayerCamera(Actor * in_targetActor)
	:Camera(in_targetActor)
	, m_targetDist(300.0f)
	, m_springConstant(64.0f)
{
}

// �f�X�g���N�^
PlayerCamera::~PlayerCamera()
{
}

// �J�������X�V
void PlayerCamera::UpdateActor(float in_deltaTime)
{

	// �΂˒萔���猸�����v�Z
	float dampening = 3.0f * Math::Sqrt(m_springConstant);

	// ���z�̈ʒu���v�Z
	Vector3 idealPos = ComputeCameraPos();

	// ���ۂƗ��z�̍����v�Z
	Vector3 diff = m_position - idealPos;
	// �΂˂ɂ������x
	Vector3 accel = -m_springConstant * diff - dampening * m_velocity;

	// ���x�̍X�V
	m_velocity += accel * in_deltaTime;
	// ���ۂ̃J�����ʒu���X�V
	m_position += m_velocity * in_deltaTime;
	// m_actualPos.x -= 20.0f;


	// �^�[�Q�b�g�͏��L�A�N�^�[�̑O���Ɉʒu���闣�ꂽ�ڕW�_
	m_viewTarget = m_targetActor->GetPosition() + m_targetActor->GetForward() * m_targetDist;

}

// �J�����𗝑z�l�ɃX�i�b�v
void PlayerCamera::SnapToIdeal()
{
	// ���ۂ̈ʒu�ɗ��z�̈ʒu���Z�b�g
	m_position = ComputeCameraPos();
	// ���x�̓[����
	m_velocity = Vector3::Zero;

	// �����_�ƃr���[���v�Z
	m_viewTarget = m_targetActor->GetPosition() + m_targetActor->GetForward() * m_targetDist;

	m_viewMatrix = Matrix4::CreateLookAt(m_position, m_viewTarget, Vector3::UnitZ);

	SetViewMatrix(m_viewMatrix);
}

// �J�����̃V�F�C�N(���ݎ���)
void PlayerCamera::ShakeCamera()
{
}

// �J�����̃^�[�Q�b�g�܂ł̋������Z�b�g
void PlayerCamera::SetBestCameraDist()
{
	m_horizonDist = 200.0f;
	//m_verticalDist = 95.0f;
	m_verticalDist = 105.0f;
	m_targetDist = 300.0f;
}

// �J�����̍��W���X�V���A�Ԃ�
Vector3 PlayerCamera::ComputeCameraPos() const
{
	// ���L�A�N�^�[�̏����ɂ���
	Vector3 cameraPos = m_targetActor->GetPosition();

	// �O�i�x�N�g��
	Vector3 forwardVec;
	forwardVec = m_targetActor->GetForward();                // �I�[�i�[�ł���v���C���[�̑O�i�x�N�g�����擾

	forwardVec = -m_horizonDist * forwardVec;          // �v���C���[�̑O�i�x�N�g���̔��Α��Ɉړ�
	forwardVec.z = m_verticalDist;                     // ������ݒ�

	cameraPos += forwardVec;                           // ���W�X�V

	return cameraPos;
}
