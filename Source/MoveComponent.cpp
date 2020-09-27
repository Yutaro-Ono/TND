#include "MoveComponent.h"
#include "Math.h"
#include "Actor.h"
#include "InputController.h"

MoveComponent::MoveComponent(Actor * in_owner, int in_updateOrder)
	:Component(in_owner, in_updateOrder)
	,m_padIsActive(CONTROLLER_INSTANCE.IsAvailable())
	,m_angularSpeed(0.0f)
	,m_forwardSpeed(0.0f)
{
}

void MoveComponent::Update(float in_deltaTime)
{




	//if (!Math::NearZero(m_angularSpeed))
	//{
	//	Quaternion rot = m_owner->GetRotation();
	//	float angle = m_angularSpeed * in_deltaTime;
	//	// Create quaternion for incremental rotation
	//	// (Rotate about up axis)
	//	Quaternion inc(Vector3::UnitZ, angle);
	//	// V‚µ‚¢‰ñ“]s—ñ‚ÆŒÃ‚¢‰ñ“]s—ñ‚ð˜AŒ‹
	//	rot = Quaternion::Concatenate(rot, inc);
	//	m_owner->SetRotation(rot);
	//}

	//if (!Math::NearZero(m_forwardSpeed) || !Math::NearZero(m_strafeSpeed))
	//{
	//	Vector3 pos = m_owner->GetPosition();
	//	pos += m_owner->GetForward() * m_forwardSpeed * in_deltaTime;
	//	pos += m_owner->GetRight() * m_strafeSpeed * in_deltaTime;
	//	m_owner->SetPosition(pos);
	//}
}
