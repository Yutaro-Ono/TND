#include "HumanCamera.h"
#include "PlayerHuman.h"

HumanCamera::HumanCamera(PlayerHuman* in_owner)
	:CameraComponent(in_owner)
	,m_pitch(0.0f)
	,m_pitchSpeed(0.0f)
	,m_maxPitch(Math::Pi / 3.0f)
{

}

HumanCamera::~HumanCamera()
{
}

// �X�V����
void HumanCamera::Update(float in_deltaTime)
{
	// �I�[�i�[�A�N�^�̍��W���擾
	Vector3 cameraPos = m_owner->GetPosition();

	// �s�b�`���I�[�i�[���ōX�V���ꂽ���x����X�V
	m_pitch += m_pitchSpeed * in_deltaTime;
	// �ő�E�ŏ��l�Ƀs�b�`���N�����v
	m_pitch = Math::Clamp(m_pitch, -m_maxPitch, m_maxPitch);
	// �s�b�`����Quaternion���쐬
	Quaternion pitchRot = Quaternion::Quaternion(m_owner->GetRight(), m_pitch);

	// �I�[�i�[�A�N�^�̑O�i�x�N�g���ƃs�b�`��Quaternion����A�J�����̑O�i�x�N�g�����Z�o
	Vector3 viewForward = Vector3::Transform(m_owner->GetForward(), pitchRot);

	// 
	Vector3 target = cameraPos + viewForward * 100.0f;
	// ������x�N�g�����쐬
	Vector3 upVec = Vector3::Transform(Vector3::UnitZ, pitchRot);

	// �r���[�s����쐬���A�����_���[�փZ�b�g
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, upVec);
	SetViewMatrix(view);
}
