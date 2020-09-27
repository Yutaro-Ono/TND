#include "LookDownCamera.h"

LookDownCamera::LookDownCamera(Actor * in_targetActor)
	:Camera(in_targetActor)
{
	m_cameraHeight = 130;
	m_cameraBack = 250;
}

LookDownCamera::~LookDownCamera()
{
}

void LookDownCamera::UpdateActor(float in_deltaTime)
{

	// �ڋߌW��
	const float dumper = 1.5f;

	// �����_�ƃL�������W�̍�
	Vector3 diff;
	Vector3 playerPos = m_targetActor->GetPosition();

	// �����x�N�g������v���C���[�ʒu�܂ŋߕt����
	diff = playerPos - m_viewTarget;
	diff = dumper * in_deltaTime * diff;

	// �����_�E�J�����ʒu���Z�b�g
	m_viewTarget = m_viewTarget + diff;
	m_position = m_viewTarget;
	m_position.x -= m_cameraBack;
	m_position.z = m_cameraHeight;

}
