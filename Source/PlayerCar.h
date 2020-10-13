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

	// 運転時の状態
	typedef enum DRIVE_STATE
	{
		DRIVE_IDLE,
		DRIVE_ACCEL,
		DRIVE_BRAKE
	};

	// どちらへ曲がっているかの状態
	typedef enum TURNING_STATE
	{
		TURN_IDLE,
		TURN_LEFT,
		TURN_RIGHT
	};

	PlayerCar();
	~PlayerCar();


	void UpdateActor(float in_deltaTime) override;

	// 人間操作モードから切り替わった時の処理
	void OnChange();

	void CollisionFix(class BoxCollider* in_hitPlayerBox, class BoxCollider* in_hitBox);

	//--------------------------------------------+
    // Getter / Setter
    //--------------------------------------------+
	void SetActive(bool in_active) { m_isActive = in_active; }

	// 車の運転状態
	void SetDriveState(DRIVE_STATE in_state) { m_driveState = in_state; }          // 車の運転状態セット
	DRIVE_STATE GetDriveState() { return m_driveState; }                           // 車の運転状態取得
	// 車の旋回状態
	void SetTurnState(TURNING_STATE in_state) { m_turnState = in_state; }          // 車の旋回状態セット
	TURNING_STATE GetTurnState() { return m_turnState; }                           // 車の運転状態取得
	// ボックスコライダーのゲッターセッター
	void SetBoxCollider(class BoxCollider* in_hitBox) { m_hitBox = in_hitBox; }
	class BoxCollider* GetBoxCollider() { return m_hitBox; }

	class MoveComponentCar* GetMoveComponent() const { return m_moveComp; }
	const float GetRadian() { return m_moveComp->GetRadian(); }                    // 弧度ゲッター (MoveComponentCar)

	// マネージャークラスポインタのゲッター・セッター
	class PlayerManager* GetPlayerManager() { return m_manager; }
	void SetPlayerManager(class PlayerManager* in_manager) { m_manager = in_manager; }

private:

	DRIVE_STATE m_driveState;             // 車の運転状態

	TURNING_STATE m_turnState;            // 車の旋回状態(どちらの向きに曲がろうとしているか)

	bool m_isActive;      // 車両操作がアクティブの時、更新処理をする
	
	// マネージャークラス
	class PlayerManager* m_manager;

	// 車のパーツクラス
	class CarBody* m_body;
	class CarDoor* m_door[2];
	class CarWheel* m_wheel[4];
	class CarHandle* m_handle;

	// MoveComponent(車用)
	class MoveComponentCar* m_moveComp;

	// CameraComponent
	class ThirdPersonCamera* m_cameraComp;

	// 当たり判定ボックス
	class BoxCollider* m_hitBox;
	class BoxCollider* m_hitGroundBox;
	class BoxCollider* m_hitHeadBox;


	static const std::string CAR_BODY_MESH_PATH;
	static const std::string CAR_DOOR_RIGHT_MESH_PATH;
	static const std::string CAR_DOOR_LEFT_MESH_PATH;
	static const std::string CAR_WHEEL_MESH_PATH;
	static const std::string CAR_HANDLE_MESH_PATH;
};