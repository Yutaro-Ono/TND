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
#include "LevelTerrain.h"
#include <string>

class PlayerCar : public Actor
{
public:

	// �^�]���̏��
	enum DRIVE_STATE
	{
		DRIVE_IDLE,
		DRIVE_ACCEL,
		DRIVE_BRAKE
	};

	// �ǂ���֋Ȃ����Ă��邩�̏��
	enum TURNING_STATE
	{
		TURN_IDLE,
		TURN_LEFT,
		TURN_RIGHT
	};

	// ���ݗ����Ă���n�`�̎��
	enum ON_TERRAIN_TYPE
	{
		ON_STREET,
		ON_GLASS
	};

	PlayerCar();
	~PlayerCar();


	void UpdateActor(float in_deltaTime) override;

	// �l�ԑ��샂�[�h����؂�ւ�������̏���
	void OnChange();

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
	// ���ݗ����Ă���n�`�̃^�C�v
	void SetOnTerrainType(LevelTerrain::TERRAIN_TYPE in_type) { m_terrainType = in_type; }
	LevelTerrain::TERRAIN_TYPE GetOnTerrainType() { return m_terrainType; }

	// �{�b�N�X�R���C�_�[�̃Q�b�^�[�Z�b�^�[
	void SetBoxCollider(class BoxCollider* in_hitBox) { m_hitBox = in_hitBox; }
	class BoxCollider* GetBoxCollider() { return m_hitBox; }
	// ���[�u�R���|�[�l���g�̃Q�b�^�[
	class MoveComponentCar* GetMoveComponent() const { return m_moveComp; }
	const float GetRadian() { return m_moveComp->GetRadian(); }                    // �ʓx�Q�b�^�[ (MoveComponentCar)

	// �}�l�[�W���[�N���X�|�C���^�̃Q�b�^�[�E�Z�b�^�[
	class PlayerManager* GetPlayerManager() { return m_manager; }
	void SetPlayerManager(class PlayerManager* in_manager) { m_manager = in_manager; }

	// �O�l�̃J�����̃Q�b�^�[
	//class ThirdPersonCamera* GetCamera() { return m_cameraComp; }
	class ThirdPersonCarCamera* GetCamera() { return m_cameraComp; }

	// �A�N�Z���l�̃Q�b�^�[
	float GetAccelValue() { return m_moveComp->GetAccelValue(); }
	// �u���[�L�l�̃Q�b�^�[
	float GetBrakeValue() { return m_moveComp->GetBrakeValue(); }

	// ���C�͂̃Q�b�^�[
	float GetFrictionVal() { return m_friction; }


private:

	DRIVE_STATE m_driveState;             // �Ԃ̉^�]���
	TURNING_STATE m_turnState;            // �Ԃ̐�����(�ǂ���̌����ɋȂ��낤�Ƃ��Ă��邩)
	LevelTerrain::TERRAIN_TYPE m_terrainType;          // ���ݗ����Ă���n�`�̃^�C�v

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

	// SoundComponent
	class CarSoundComponent* m_soundComp;

	// �����蔻��{�b�N�X
	class BoxCollider* m_hitBox;
	class BoxCollider* m_hitGroundBox;
	class BoxCollider* m_hitHeadBox;

	// ���C��
	float m_friction;





	static const std::string CAR_HANDLE_MESH_PATH;
};