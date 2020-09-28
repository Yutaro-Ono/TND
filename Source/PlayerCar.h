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

	PlayerCar();
	~PlayerCar();


	void UpdateActor(float in_deltaTime) override;

	void CreateAABB(class Mesh* in_mesh);

	void CollisionFix(class BoxCollider* in_hitPlayerBox, class BoxCollider* in_hitBox);

	//--------------------------------------------+
    // Getter / Setter
    //--------------------------------------------+
	void SetActive(bool in_active) { m_isActive = in_active; }

private:

	bool m_isActive;      // �ԗ����삪�A�N�e�B�u�̎��A�X�V����������
	
	// �Ԃ̃p�[�c�N���X
	class CarBody* m_body;
	class CarDoor* m_door[2];
	class CarWheel* m_wheel[4];
	class CarHandle* m_handle;

	// MoveComponent(�ԗp)
	class MoveComponentCar* m_moveComp;

	// CameraComponent
	class OrbitCamera* m_cameraComp;

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