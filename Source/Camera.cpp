//-----------------------------------------------------------------------+
// カメラクラス
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Camera.h"
#include "GameMain.h"
#include "Renderer.h"

// コンストラクタ
Camera::Camera(Actor * in_targetActor)
	:m_targetActor(in_targetActor)
{
	// m_position = Vector3(0, 100, 100);

	// デフォルトカメラ値セット
	m_viewMatrix = Matrix4::CreateLookAt(Vector3(-100, 0, 100), Vector3(0, 0, 0), Vector3(0, 0, 1));

	// ゲームシステム側にこれがアクティブカメラであることを通知
	SetActive();
}

// デストラクタ
Camera::~Camera()
{
	// ゲームシステム側にアクティブカメラがいなくなったことを通知
	// カメラ自体は継承元のActorでデストラクトされる
	GAME_INSTANCE.InActiveCamera(this);
}

void Camera::UpdateActor(float in_deltaTime)
{
	
}

void Camera::Update(float in_deltaTime)
{
	UpdateActor(in_deltaTime);

	// カメラ視線ベクトル、カメラ行列作成
	m_viewVector = m_viewTarget - m_position;
	m_viewMatrix = Matrix4::CreateLookAt(m_position, m_viewTarget, Vector3::UnitZ);
}

// ゲームシステム側にこれがアクティブカメラであることを手動で通知する
void Camera::SetActive()
{
	// ゲームシステム側にこれがアクティブカメラであることを通知
	GAME_INSTANCE.SetCamera(this);
}

// 位置方向初期化
void Camera::Initialize(const Vector3 & in_cameraPos, const Vector3 & in_targetPos, const Vector3 & in_upVec)
{
	m_position = in_cameraPos;
	m_viewTarget = in_targetPos;
	m_viewVector = in_targetPos - in_cameraPos;

	m_viewMatrix = Matrix4::CreateLookAt(in_cameraPos, in_targetPos, in_upVec);
}

void Camera::SetViewMatrix(const Matrix4 & in_view)
{
	GAME_INSTANCE.GetRenderer()->SetViewMatrix(in_view);
}
