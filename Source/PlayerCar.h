#pragma once
#include "Actor.h"
#include "CarBody.h"
#include "CarDoor.h"
#include "CarWheel.h"
#include "CarHandle.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "MoveComponentCar.h"
#include <string>

class PlayerCar : public Actor
{
public:

	// �^�]���̏��
	typedef enum DRIVE_STATE
	{
		DRIVE_IDLE,
		DRIVE_ACCEL,
		DRIVE_BRAKE
	};

	// �ǂ���֋Ȃ����Ă��邩�̏��
	typedef enum TURNING_STATE
	{
		TURN_IDLE,
		TURN_LEFT,
		TURN_RIGHT
	};

	PlayerCar();
	~PlayerCar();


	void UpdateActor(float in_deltaTime) override;

	void CollisionFix(class BoxCollider* in_hitPlayerBox, class BoxCollider* in_hitBox);

	//--------------------------------------------+
    // Getter / Setter
    //--------------------------------------------+
	void SetActive(bool in_active) { m_isActive = in_active; }

	// �Ԃ̉^�]���
	void SetDriveState(DRIVE_STATE in_state) { m_driveState = in_state; }          // �Ԃ̉^�]��ԃZ�b�g
	DRIVE_STATE GetDriveState() { return m_driveState; }                           // �Ԃ̉^�]��Ԏ擾
	// �Ԃ̐�����
	void SetTurnState(TURNING_STATE in_state) { m_turnState = in_state; }          // �Ԃ̐����ԃZ�b�g
	TURNING_STATE GetTurnState() { return m_turnState; }                           // �Ԃ̉^�]��Ԏ擾
	// �{�b�N�X�R���C�_�[�̃Q�b�^�[�Z�b�^�[
	void SetBoxCollider(class BoxCollider* in_hitBox) { m_hitBox = in_hitBox; }
	class BoxCollider* GetBoxCollider() { return m_hitBox; }

	class MoveComponentCar* GetMoveComponent() const { return m_moveComp; }
	const float GetRadian() { return m_moveComp->GetRadian(); }                    // �ʓx�Q�b�^�[ (MoveComponentCar)

	// �}�l�[�W���[�N���X�|�C���^�̃Q�b�^�[�E�Z�b�^�[
	class PlayerManager* GetPlayerManager() { return m_manager; }
	void SetPlayerManager(class PlayerManager* in_manager) { m_manager = in_manager; }

private:

	DRIVE_STATE m_driveState;             // �Ԃ̉^�]���

	TURNING_STATE m_turnState;            // �Ԃ̐�����(�ǂ���̌����ɋȂ��낤�Ƃ��Ă��邩)

	bool m_isActive;      // �ԗ����삪�A�N�e�B�u�̎��A�X�V����������
	
	// �}�l�[�W���[�N���X
	class PlayerManager* m_manager;

	// �Ԃ̃p�[�c�N���X
	class CarBody* m_body;
	class CarDoor* m_door[2];
	class CarWheel* m_wheel[4];
	class CarHandle* m_handle;

	// MoveComponent(�ԗp)
	class MoveComponentCar* m_moveComp;

	// CameraComponent
	class ThirdPersonCarCamera* m_cameraComp;

	// �����蔻��{�b�N�X
	class BoxCollider* m_hitBox;
	class BoxCollider* m_hitGroundBox;
	class BoxCollider* m_hitHeadBox;


	static const std::string CAR_BODY_MESH_PATH;
	static const std::string CAR_DOOR_RIGHT_MESH_PATH;
	static const std::string CAR_DOOR_LEFT_MESH_PATH;
	static const std::string CAR_WHEEL_MESH_PATH;
	static const std::string CAR_HANDLE_MESH_PATH;
};