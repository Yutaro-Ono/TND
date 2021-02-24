#include "ResultCar.h"
#include "ResultCarBody.h"
#include "ResultCarDoor.h"
#include "ResultCarWheel.h"
#include "ResultCarHandle.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "PointCameraComponent.h"


ResultCar::ResultCar()
	:m_accel(450.0f)
	,m_nextScene(false)
	,m_cameraPosType(NORMAL)
{
	// 各パーツごとのクラスを作成
	m_body = new ResultCarBody(this);
	m_door[0] = new ResultCarDoor(this, ResultCarDoor::DOOR_POS::LEFT);
	m_door[1] = new ResultCarDoor(this, ResultCarDoor::DOOR_POS::RIGHT);
	m_handle = new ResultCarHandle(this, "Data/Meshes/TND/Actors/Car/Player/Handle/SM_suv_steering_wheel_lod0_Internal.OBJ");
	// タイヤ
	m_wheel[0] = new ResultCarWheel(this, ResultCarWheel::WHEEL_POSITION::FRONT_LEFT);
	m_wheel[1] = new ResultCarWheel(this, ResultCarWheel::WHEEL_POSITION::FRONT_RIGHT);
	m_wheel[2] = new ResultCarWheel(this, ResultCarWheel::WHEEL_POSITION::BACK_LEFT);
	m_wheel[3] = new ResultCarWheel(this, ResultCarWheel::WHEEL_POSITION::BACK_RIGHT);

	// カメラ
	m_camera = new PointCameraComponent(this, Vector3(65.0f, 75.0f, 60.0f));
	m_camera->SetMoveVec(Vector3(0.0f, 0.8f, 0.0f));
	m_camera->SetTargetOffset(Vector3(30.0f, 0.0f, 20.0f));
}

ResultCar::~ResultCar()
{
}

void ResultCar::UpdateActor(float in_deltaTime)
{

	// 自動で前進
	Vector3 move = Vector3(m_position.x + m_accel * in_deltaTime, m_position.y, m_position.z);

	SetPosition(move);


	// 8秒経過でカメラ切り替え
	if (m_nextScene)
	{
		if (m_cameraPosType == NORMAL)
		{
			m_camera->SetMoveVec(Vector3(0.0f, 3.5f, 0.0f));
			m_cameraPosType = END;
		}
		else if (m_cameraPosType == END)
		{
			m_camera->SetOffset(Vector3(-210.0f, -62.0f, 58.0f), Vector3::UnitZ);
		}
		
	}

	if (m_position.x > 25000)
	{
		m_position.x = 6500.0f;
	}

}
