#include "PlayerCar.h"
#include "MoveComponentCar.h"
#include "ThirdPersonCarCamera.h"
#include "Input.h"
#include "InputController.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "PhysicsWorld.h"

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
	// �J�����R���|�[�l���g�𐶐�
	m_cameraComp = new ThirdPersonCarCamera(this);

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


	// �f�B�A�N�e�B�x�[�g���ꂽ��
	if (!m_isActive)
	{
		// ���b�V���̕\����؂点��悤����
		m_body->GetMeshComponent()->SetVisible(false);
		m_door[0]->GetMeshComponent()->SetVisible(false);
		m_door[1]->GetMeshComponent()->SetVisible(false);
		m_handle->GetMeshComponent()->SetVisible(false);
		for (int i = 0; i < 4; i++)
		{
			m_wheel[i]->GetMeshComponent()->SetVisible(false);
		}

	}
	else
	{
		m_body->GetMeshComponent()->SetVisible(true);
		m_door[0]->GetMeshComponent()->SetVisible(true);
		m_door[1]->GetMeshComponent()->SetVisible(true);
		m_handle->GetMeshComponent()->SetVisible(true);
		for (int i = 0; i < 4; i++)
		{
			m_wheel[i]->GetMeshComponent()->SetVisible(true);
		}
	}

}

// �󂯎�������b�V�����瓖���蔻��{�b�N�X�𐶐�
void PlayerCar::CreateAABB(Mesh* in_mesh)
{
	// �����蔻��Z�b�g
	AABB playerBox = in_mesh->GetCollisionBox();
	// ���Ԃ̃��b�V������͍��W�����Ȃ����߁A�蓮�ŃZ�b�g
	playerBox.SetMinVector(Vector3(-50.0f, -50.0f, -50.0f));
	playerBox.SetMaxVector(Vector3(50.0f, 50.0f, 50.0f));
	m_hitBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	playerBox.m_min.x *= 1.0f;
	playerBox.m_min.y *= 1.0f;
	playerBox.m_max.x *= 1.0f;
	playerBox.m_max.y *= 1.0f;
	m_hitBox->SetObjectBox(playerBox);
}

// �Փˎ��̉����o������
void PlayerCar::CollisionFix(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
{
	Vector3 fix;


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
