#include "PlayerCar.h"
#include "MoveComponentCar.h"
#include "ThirdPersonCarCamera.h"
#include "ThirdPersonCamera.h"
#include "Input.h"
#include "InputController.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "Component.h"
#include "PhysicsWorld.h"
#include "PlayerManager.h"
#include "LevelTerrain.h"

const std::string PlayerCar::CAR_HANDLE_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Handle/SM_suv_steering_wheel_lod1_Internal.OBJ";

// �R���X�g���N�^
PlayerCar::PlayerCar()
	:m_isActive(true)
	,m_driveState(DRIVE_IDLE)
	,m_turnState(TURN_IDLE)
	,m_friction(1.0f)
{
	// �ԗ�����p��MoveComponent�𐶐�
	m_moveComp = new MoveComponentCar(this);
	m_moveComp->SetActive(false);
	// �J�����R���|�[�l���g�𐶐�
	m_cameraComp = new ThirdPersonCamera(this);
	m_cameraComp->SetAdjustForward(false);
	m_cameraComp->SetChaseOwnerForward(false);
	m_cameraComp->SetDistance(200.0f);

	// �e�p�[�c���Ƃ̃N���X���쐬
	m_body = new CarBody(this);
	m_door[0] = new CarDoor(this, CarDoor::DOOR_POS::LEFT);
	m_door[1] = new CarDoor(this, CarDoor::DOOR_POS::RIGHT);
	m_handle = new CarHandle(this, CAR_HANDLE_MESH_PATH);
	// �^�C��
	m_wheel[0] = new CarWheel(this, CarWheel::WHEEL_POSITION::FRONT_LEFT);
	m_wheel[1] = new CarWheel(this, CarWheel::WHEEL_POSITION::FRONT_RIGHT);
	m_wheel[2] = new CarWheel(this, CarWheel::WHEEL_POSITION::BACK_LEFT);
	m_wheel[3] = new CarWheel(this, CarWheel::WHEEL_POSITION::BACK_RIGHT);



}

PlayerCar::~PlayerCar()
{

}

void PlayerCar::UpdateActor(float in_deltaTime)
{

	// �v���C���[���Ԃ��^�]���Ă����Ԃ̎�
	if (m_manager->GetPlayerMode() == PlayerManager::PLAYER_MODE::MODE_CAR)
	{


		// ���x�����ȏォ�A�N�Z���𓥂�ł��鎞�A�J�����̒Ǐ]���I���ɂ���
		if (m_moveComp->GetAccelValue() >= 30.0f)
		{
			//m_cameraComp->SetChaseOwnerForward(true);
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


// �l�ԑ��삩��؂�ւ�������̊e��ύX����
void PlayerCar::OnChange()
{
	GAME_INSTANCE.SetCamera(m_cameraComp);
	m_cameraComp->SetDistance(200.0f);
	m_moveComp->SetActive(true);
}

// �Փˎ��̉����o������
void PlayerCar::CollisionFix(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
{
	Vector3 fix = Vector3::Zero;

	//�ǂƂԂ������Ƃ�
	AABB bgBox = in_hitBox->GetWorldBox();
	AABB playerBox = m_hitBox->GetWorldBox();


	// ���̓����蔻��{�b�N�X�Ȃ�
	if (in_hitBox->GetPhysicsType() == PhysicsWorld::TYPE_TERRAIN)
	{
		if (bgBox.Contains(m_position - Vector3(0.0f, 0.0f, 10.0f)))
		{
			// �n�`���Ƃɖ��C�͂�ݒ�
			if (in_hitBox->GetTerrainPtr()->GetNodeType() == LevelTerrain::TYPE_GLASS)
			{
				m_friction = in_hitBox->GetTerrainPtr()->GetFrictionVal();
			}
			if (in_hitBox->GetTerrainPtr()->GetNodeType() == LevelTerrain::TYPE_STREET)
			{
				m_friction = in_hitBox->GetTerrainPtr()->GetFrictionVal();
			}
		}

	}


	// �߂荞�݂��C��
	CalcCollisionFixVec(playerBox, bgBox, fix);

	// �␳�x�N�g�����߂�
	m_position += fix;

	// �Փˎ��̏���
	if (fix.x > 5.0f || fix.x < -5.0f || fix.y > 5.0f || fix.y < -5.0f)
	{
		// �A�N�Z������ (�Փˎ����O�̔����̃X�s�[�h�ɂ���)
		m_moveComp->SetAccel(m_moveComp->GetAccelValue() / 2.0f);
	}


	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	m_hitBox->OnUpdateWorldTransform();

	// printf("[%f, %f, %f]\n", m_position.x, m_position.y, m_position.z);
}
