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
	:m_accel(450.0f)
	,m_nowTime(SDL_GetTicks())
	,m_cameraPosType(NONE)
{
	// �e�p�[�c���Ƃ̃N���X���쐬
	m_body = new TitleCarBody(this);
	m_door[0] = new TitleCarDoor(this, TitleCarDoor::DOOR_POS::LEFT);
	m_door[1] = new TitleCarDoor(this, TitleCarDoor::DOOR_POS::RIGHT);
	m_handle = new TitleCarHandle(this, "Data/Meshes/TND/Actors/Car/Player/Handle/SM_suv_steering_wheel_lod0_Internal.OBJ");
	// �^�C��
	m_wheel[0] = new TitleCarWheel(this, TitleCarWheel::WHEEL_POSITION::FRONT_LEFT);
	m_wheel[1] = new TitleCarWheel(this, TitleCarWheel::WHEEL_POSITION::FRONT_RIGHT);
	m_wheel[2] = new TitleCarWheel(this, TitleCarWheel::WHEEL_POSITION::BACK_LEFT);
	m_wheel[3] = new TitleCarWheel(this, TitleCarWheel::WHEEL_POSITION::BACK_RIGHT);

	// �J����
	m_camera = new PointCameraComponent(this, Vector3(180.0f, -69.0f, 75.0f));

}

TitleCar::~TitleCar()
{
}

void TitleCar::UpdateActor(float in_deltaTime)
{

	m_position.z -= 1.0f * in_deltaTime;
	if (m_position.z < -0.8f)
	{
		m_position.z = 0.0f;
	}
	// �����őO�i
	Vector3 move = Vector3(m_position.x + m_accel * in_deltaTime, m_position.y, m_position.z);


	SetPosition(move);

	// �J�����؂�ւ��ɂ����鎞��(�b)
	int changeSec = 8000;

	if (m_cameraPosType == FAR_FRONT || m_cameraPosType == UNDER)
	{
		changeSec = 3500;
	}

	// 8�b�o�߂ŃJ�����؂�ւ�
	if (m_nowTime + changeSec < SDL_GetTicks())
	{
		if (m_cameraPosType == NONE)
		{
			m_cameraPosType = FRONT_LEFT;
		}
		else if (m_cameraPosType == FRONT_LEFT)
		{
			m_camera->SetOffset(Vector3(-210.0f, -62.0f, 58.0f));
			m_cameraPosType = BACK_LEFT;
		}
		else if (m_cameraPosType == BACK_LEFT)
		{
			m_camera->SetOffset(Vector3(0.0f, -195.0f, 65.0f));
			m_cameraPosType = SIDE;
		}
		else if (m_cameraPosType == SIDE)
		{
			m_camera->SetOffset(Vector3(m_position.x + 1200.0f, 0.0f, 30.0f));
			m_camera->SetCameraChaseType(PointCameraComponent::CAMERA_CHASE::DISABLE);
			m_cameraPosType = FAR_FRONT;
		}
		else if (m_cameraPosType == FAR_FRONT)
		{
			m_camera->SetOffset(Vector3(180.0f, 82.0f, 75.0f));
			m_camera->SetCameraChaseType(PointCameraComponent::CAMERA_CHASE::ENABLE);
			m_cameraPosType = FRONT_RIGHT;
		}
		else if (m_cameraPosType == FRONT_RIGHT)
		{
			m_camera->SetOffset(Vector3(-210.0f, 82.0f, 58.0f));
			m_cameraPosType = BACK_RIGHT;
		}
		else if (m_cameraPosType == BACK_RIGHT)
		{
			m_camera->SetOffset(Vector3(-50.0f, 0.0f, 450.0f));
			m_cameraPosType = TOP;
		}
		else if (m_cameraPosType == TOP)
		{
			m_camera->SetOffset(Vector3(m_position.x + 1500.0f, m_position.y, 10.0f));
			m_camera->SetCameraChaseType(PointCameraComponent::CAMERA_CHASE::DISABLE);
			m_cameraPosType = UNDER;
		}
		else if (m_cameraPosType == UNDER)
		{
			m_camera->SetOffset(Vector3(-210.0f, -62.0f, 58.0f));
			m_camera->SetCameraChaseType(PointCameraComponent::CAMERA_CHASE::ENABLE);
			m_cameraPosType = FRONT_LEFT;
		}
		

		m_nowTime = SDL_GetTicks();
	}

	if (m_position.x > 25000)
	{
		m_position.x = 0.0f;
	}

}
