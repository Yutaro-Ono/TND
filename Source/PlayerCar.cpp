#include "PlayerCar.h"
#include "MoveComponentCar.h"
#include "ThirdPersonCarCamera.h"
#include "ThirdPersonCamera.h"
#include "Input.h"
#include "InputController.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "PhysicsWorld.h"
#include "PlayerManager.h"

const std::string PlayerCar::CAR_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/SM_suv_parts_LOD1_body_Internal.OBJ";
const std::string PlayerCar::CAR_DOOR_LEFT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/SM_suv_parts_LOD1_left_door_Internal.OBJ";
const std::string PlayerCar::CAR_DOOR_RIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/SM_suv_parts_LOD1_right_door_Internal.OBJ";
const std::string PlayerCar::CAR_WHEEL_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Wheel/SM_suv_parts_LOD0_wheel_Internal.OBJ";
const std::string PlayerCar::CAR_HANDLE_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Handle/SM_suv_steering_wheel_lod1_Internal.OBJ";

// �R���X�g���N�^
PlayerCar::PlayerCar()
	:m_isActive(true)
	,m_driveState(DRIVE_IDLE)
	,m_turnState(TURN_IDLE)
{
	// �ԗ�����p��MoveComponent�𐶐�
	m_moveComp = new MoveComponentCar(this);
	m_moveComp->SetActive(false);
	// �J�����R���|�[�l���g�𐶐�
	m_cameraComp = new ThirdPersonCamera(this);
	m_cameraComp->SetAdjustForward(false);
	m_cameraComp->SetChaseOwnerForward(false);

	// �e�p�[�c���Ƃ̃N���X���쐬
	m_body = new CarBody(this, CAR_BODY_MESH_PATH);
	m_door[0] = new CarDoor(this, CAR_DOOR_LEFT_MESH_PATH, true);
	m_door[1] = new CarDoor(this, CAR_DOOR_RIGHT_MESH_PATH, false);
	m_handle = new CarHandle(this, CAR_HANDLE_MESH_PATH);
	// �^�C��
	m_wheel[0] = new CarWheel(this, CAR_WHEEL_MESH_PATH, CarWheel::WHEEL_POSITION::FRONT_LEFT);
	m_wheel[1] = new CarWheel(this, CAR_WHEEL_MESH_PATH, CarWheel::WHEEL_POSITION::FRONT_RIGHT);
	m_wheel[2] = new CarWheel(this, CAR_WHEEL_MESH_PATH, CarWheel::WHEEL_POSITION::BACK_LEFT);
	m_wheel[3] = new CarWheel(this, CAR_WHEEL_MESH_PATH, CarWheel::WHEEL_POSITION::BACK_RIGHT);



}

PlayerCar::~PlayerCar()
{
}

void PlayerCar::UpdateActor(float in_deltaTime)
{

	// �v���C���[���Ԃ��^�]���Ă����Ԃ̎�
	if (m_manager->GetPlayerMode() == PlayerManager::PLAYER_MODE::MODE_CAR)
	{
		GAME_INSTANCE.SetCamera(m_cameraComp);
		m_moveComp->SetActive(true);

		// ���x�����ȏォ�A�N�Z���𓥂�ł��鎞�A�J�����̒Ǐ]���I���ɂ���
		if (m_moveComp->GetAccelValue() >= 30.0f)
		{
			m_cameraComp->SetChaseOwnerForward(true);
		}
		else
		{
			m_cameraComp->SetChaseOwnerForward(false);
		}
	}
	else
	{
		m_moveComp->SetActive(false);
	}



}



// �Փˎ��̉����o������
void PlayerCar::CollisionFix(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
{
	Vector3 fix = Vector3::Zero;


	//�ǂƂԂ������Ƃ�
	AABB bgBox = in_hitBox->GetWorldBox();
	AABB playerBox = m_hitBox->GetWorldBox();

	// �߂荞�݂��C��
	CalcCollisionFixVec(playerBox, bgBox, fix);

	// �␳�x�N�g�����߂�
	m_position += fix;

	// �ǏՓˎ��̏���
	if (fix.x > 5.0f || fix.x < -5.0f || fix.y > 5.0f || fix.y < -5.0f)
	{

		// �A�N�Z������ (�Փˎ����O�̔����̃X�s�[�h�ɂ���)
		m_moveComp->SetAccel(m_moveComp->GetAccelValue() / 2.0f);
	}


	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	m_hitBox->OnUpdateWorldTransform();

	// printf("[%f, %f, %f]\n", m_position.x, m_position.y, m_position.z);
}
