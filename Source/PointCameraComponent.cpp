#include "PointCameraComponent.h"
#include "Actor.h"

PointCameraComponent::PointCameraComponent(Actor* in_owner, const Vector3& in_offset)
	:CameraComponent(in_owner)
	,m_offset(in_offset)
	,m_chaseType(ENABLE)
{
	// プロジェクション行列の調整・変更
	ChangeProjectionMat(80.0f, 1.0f, 10000.0f);
	// ビュー行列のセット
	m_view = Matrix4::CreateLookAt(m_owner->GetPosition() + m_offset, m_owner->GetPosition(), Vector3::UnitZ);
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
		m_offset.y += 2.5f * in_deltaTime;

		m_view = Matrix4::CreateLookAt(m_owner->GetPosition() + m_offset, m_owner->GetPosition(), Vector3::UnitZ);
		SetViewMatrix(m_view);
	}
	else if (m_chaseType == DISABLE)
	{
		m_view = Matrix4::CreateLookAt(m_offset, m_owner->GetPosition(), Vector3::UnitZ);
		SetViewMatrix(m_view);
	}


}

// カメラオフセットを変更し、ビュー行列を更新
void PointCameraComponent::SetOffset(const Vector3& in_offset)
{
	m_offset = in_offset;
	m_view = Matrix4::CreateLookAt(m_owner->GetPosition() + m_offset, m_owner->GetPosition(), Vector3::UnitZ);
	SetViewMatrix(m_view);
}
