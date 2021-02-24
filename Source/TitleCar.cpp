#include "TitleCar.h"
#include "TitleCarBody.h"
#include "TitleCarDoor.h"
#include "TitleCarWheel.h"
#include "TitleCarHandle.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "PointCameraComponent.h"


TitleCar::TitleCar()
	:m_accel(550.0f)
	,m_nowTime(SDL_GetTicks())
	,m_cameraPosType(NONE)
{
	// 各パーツごとのクラスを作成
	m_body = new TitleCarBody(this);
	m_door[0] = new TitleCarDoor(this, TitleCarDoor::DOOR_POS::LEFT);
	m_door[1] = new TitleCarDoor(this, TitleCarDoor::DOOR_POS::RIGHT);
	m_handle = new TitleCarHandle(this, "Data/Meshes/TND/Actors/Car/Player/Handle/SM_suv_steering_wheel_lod0_Internal.OBJ");
	// タイヤ
	m_wheel[0] = new TitleCarWheel(this, TitleCarWheel::WHEEL_POSITION::FRONT_LEFT);
	m_wheel[1] = new TitleCarWheel(this, TitleCarWheel::WHEEL_POSITION::FRONT_RIGHT);
	m_wheel[2] = new TitleCarWheel(this, TitleCarWheel::WHEEL_POSITION::BACK_LEFT);
	m_wheel[3] = new TitleCarWheel(this, TitleCarWheel::WHEEL_POSITION::BACK_RIGHT);

	// カメラ
	m_camera = new PointCameraComponent(this, Vector3(180.0f, -69.0f, 75.0f));

}

TitleCar::~TitleCar()
{
}

void TitleCar::UpdateActor(float in_deltaTime)
{

	// 自動で前進
	Vector3 move = Vector3(m_position.x + m_accel * in_deltaTime, m_position.y, m_position.z);


	SetPosition(move);

	// カメラ切り替えにかかる時間(秒)
	int changeSec = 6500;

	if (m_cameraPosType == FAR_FRONT || m_cameraPosType == UNDER)
	{
		changeSec = 3500;
	}

	// 8秒経過でカメラ切り替え
	if (m_nowTime + changeSec < SDL_GetTicks())
	{
		if (m_cameraPosType == NONE)
		{
			m_camera->SetMoveVec(Vector3(0.0f, 3.5f, 0.0f));
			m_cameraPosType = FRONT_LEFT;
		}
		else if (m_cameraPosType == FRONT_LEFT)
		{
			m_camera->SetOffset(Vector3(-210.0f, -62.0f, 64.0f), Vector3::UnitZ);
			m_cameraPosType = BACK_LEFT;
		}
		else if (m_cameraPosType == BACK_LEFT)
		{
			m_camera->SetOffset(Vector3(0.0f, -245.0f, 65.0f), Vector3::UnitZ);
			m_cameraPosType = SIDE;
		}
		else if (m_cameraPosType == SIDE)
		{
			m_camera->SetOffset(Vector3(m_position.x + 1200.0f, 0.0f, 30.0f), Vector3::UnitZ);
			m_camera->SetCameraChaseType(PointCameraComponent::CAMERA_CHASE::DISABLE);
			m_cameraPosType = FAR_FRONT;
		}
		else if (m_cameraPosType == FAR_FRONT)
		{
			m_camera->SetOffset(Vector3(180.0f, 82.0f, 75.0f), Vector3::UnitZ);
			m_camera->SetCameraChaseType(PointCameraComponent::CAMERA_CHASE::ENABLE);
			m_cameraPosType = FRONT_RIGHT;
		}
		else if (m_cameraPosType == FRONT_RIGHT)
		{
			m_camera->SetOffset(Vector3(-210.0f, 82.0f, 58.0f), Vector3::UnitZ);
			m_cameraPosType = BACK_RIGHT;
		}
		else if (m_cameraPosType == BACK_RIGHT)
		{
			m_camera->SetOffset(Vector3(0.0f, 0.0f, 250.0f), Vector3::UnitX);
			m_camera->SetMoveVec(Vector3(0.0f, 0.0f, 2.5f));
			m_cameraPosType = TOP;
		}
		else if (m_cameraPosType == TOP)
		{
			m_camera->SetOffset(Vector3(m_position.x + 1500.0f, m_position.y, 10.0f), Vector3::UnitZ);
			m_camera->SetMoveVec(Vector3(0.0f, 3.5f, 0.0f));
			m_camera->SetCameraChaseType(PointCameraComponent::CAMERA_CHASE::DISABLE);
			m_cameraPosType = UNDER;
		}
		else if (m_cameraPosType == UNDER)
		{
			m_camera->SetCameraChaseType(PointCameraComponent::CAMERA_CHASE::ENABLE);
			m_camera->SetOffset(Vector3(210.0f, -62.0f, 58.0f), Vector3::UnitZ);
			m_cameraPosType = FRONT_LEFT;
		}
		

		m_nowTime = SDL_GetTicks();
	}

	if (m_position.x > 35000 && m_cameraPosType != FAR_FRONT && m_cameraPosType != UNDER)
	{
		m_position.x = 6500.0f;
	}

}
