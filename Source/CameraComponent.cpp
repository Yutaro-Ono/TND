#include "CameraComponent.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor * in_target, int updateOrder)
	:Component(in_target, updateOrder)
{
	// ゲーム側にこのカメラをアクティブなカメラとして登録
	SetActive();
}

CameraComponent::~CameraComponent()
{
	// ゲームからこのカメラを削除
	GAME_INSTANCE.InActiveCamera(this);
}

void CameraComponent::SetViewMatrix(const Matrix4 & in_view)
{
	GAME_INSTANCE.GetRenderer()->SetViewMatrix(in_view);
}
