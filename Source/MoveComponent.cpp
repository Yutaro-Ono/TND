#include "MoveComponent.h"
#include "Math.h"
#include "Actor.h"
#include "InputController.h"

MoveComponent::MoveComponent(Actor * in_owner, int in_updateOrder)
	:Component(in_owner, in_updateOrder)
	,m_isActive(true)
	,m_padIsActive(CONTROLLER_INSTANCE.IsAvailable())
	,m_angularSpeed(0.0f)
	,m_forwardSpeed(0.0f)
{
}

void MoveComponent::Update(float in_deltaTime)
{
	// �p�b�h�ڑ�����Ă��邩�̍X�V����
	m_padIsActive = CONTROLLER_INSTANCE.IsAvailable();
}
