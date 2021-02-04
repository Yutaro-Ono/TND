#include "PointCameraComponent.h"
#include "Actor.h"

PointCameraComponent::PointCameraComponent(Actor* in_owner, const Vector3& in_offset)
	:CameraComponent(in_owner)
	,m_offset(in_offset)
	,m_targetOffset(Vector3::Zero)
	,m_upVec(Vector3::UnitZ)
	,m_moveVec(Vector3(0.0f, 2.5f, 0.0f))
	,m_chaseType(ENABLE)
{
	// �v���W�F�N�V�����s��̒����E�ύX
	ChangeProjectionMat(80.0f, 1.0f, 10000.0f);
	// �r���[�s��̃Z�b�g
	m_view = Matrix4::CreateLookAt(m_owner->GetPosition() + m_offset, m_owner->GetPosition() + m_targetOffset, m_upVec);
	SetViewMatrix(m_view);
}

PointCameraComponent::~PointCameraComponent()
{
}

void PointCameraComponent::Update(float in_deltaTime)
{
	// �r���[�s��̍X�V����
	// �Ǐ]����or�Ȃ�
	if (m_chaseType == ENABLE)
	{
		m_offset += m_moveVec * in_deltaTime;

		m_view = Matrix4::CreateLookAt(m_owner->GetPosition() + m_offset, m_owner->GetPosition() + m_targetOffset, m_upVec);
		SetViewMatrix(m_view);
	}
	else if (m_chaseType == DISABLE)
	{
		m_view = Matrix4::CreateLookAt(m_offset, m_owner->GetPosition() + m_targetOffset, m_upVec);
		SetViewMatrix(m_view);
	}


}

// �J�����I�t�Z�b�g��ύX���A�r���[�s����X�V
void PointCameraComponent::SetOffset(const Vector3& in_offset, const Vector3& in_upVec)
{
	m_offset = in_offset;
	m_upVec = in_upVec;
	m_view = Matrix4::CreateLookAt(m_owner->GetPosition() + m_offset, m_owner->GetPosition() + m_targetOffset, m_upVec);
	SetViewMatrix(m_view);
}
