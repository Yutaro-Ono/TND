#include "CameraComponent.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor * in_target, int updateOrder)
	:Component(in_target, updateOrder)
	,m_position(Vector3::Zero)
{

	// プロジェクション行列セット
	Matrix4 projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(75.0f),
		static_cast<float>(GAME_CONFIG->GetScreenWidth()),
		static_cast<float>(GAME_CONFIG->GetScreenHeight()),
		1.0f, 100000.0f);
	RENDERER->SetProjectionMatrix(projection);

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
