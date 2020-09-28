#include "CameraComponent.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor * in_target, int updateOrder)
	:Component(in_target, updateOrder)
{
	// �Q�[�����ɂ��̃J�������A�N�e�B�u�ȃJ�����Ƃ��ēo�^
	SetActive();
}

CameraComponent::~CameraComponent()
{
	// �Q�[�����炱�̃J�������폜
	GAME_INSTANCE.InActiveCamera(this);
}

void CameraComponent::SetViewMatrix(const Matrix4 & in_view)
{
	GAME_INSTANCE.GetRenderer()->SetViewMatrix(in_view);
}
