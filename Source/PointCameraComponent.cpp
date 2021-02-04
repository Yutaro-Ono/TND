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
	// プロジェクション行列の調整・変更
	ChangeProjectionMat(80.0f, 1.0f, 10000.0f);
	// ビュー行列のセット
	m_view = Matrix4::CreateLookAt(m_owner->GetPosition() + m_offset, m_owner->GetPosition() + m_targetOffset, m_upVec);
	SetViewMatrix(m_view);
}

PointCameraComponent::~PointCameraComponent()
{
}

void PointCameraComponent::Update(float in_deltaTime)
{
	// ビュー行列の更新処理
	// 追従ありorなし
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

// カメラオフセットを変更し、ビュー行列を更新
void PointCameraComponent::SetOffset(const Vector3& in_offset, const Vector3& in_upVec)
{
	m_offset = in_offset;
	m_upVec = in_upVec;
	m_view = Matrix4::CreateLookAt(m_owner->GetPosition() + m_offset, m_owner->GetPosition() + m_targetOffset, m_upVec);
	SetViewMatrix(m_view);
}
