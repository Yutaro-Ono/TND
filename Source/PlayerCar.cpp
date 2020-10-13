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

// コンストラクタ
PlayerCar::PlayerCar()
	:m_isActive(true)
	,m_driveState(DRIVE_IDLE)
	,m_turnState(TURN_IDLE)
{
	// 車両操作用のMoveComponentを生成
	m_moveComp = new MoveComponentCar(this);
	m_moveComp->SetActive(false);
	// カメラコンポーネントを生成
	m_cameraComp = new ThirdPersonCamera(this);
	m_cameraComp->SetAdjustForward(false);
	m_cameraComp->SetChaseOwnerForward(false);
	m_cameraComp->SetDistance(200.0f);

	// 各パーツごとのクラスを作成
	m_body = new CarBody(this, CAR_BODY_MESH_PATH);
	m_door[0] = new CarDoor(this, CAR_DOOR_LEFT_MESH_PATH, true);
	m_door[1] = new CarDoor(this, CAR_DOOR_RIGHT_MESH_PATH, false);
	m_handle = new CarHandle(this, CAR_HANDLE_MESH_PATH);
	// タイヤ
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

	// プレイヤーが車を運転している状態の時
	if (m_manager->GetPlayerMode() == PlayerManager::PLAYER_MODE::MODE_CAR)
	{


		// 速度が一定以上かつアクセルを踏んでいる時、カメラの追従をオンにする
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


// 人間操作から切り替わった時の各種変更処理
void PlayerCar::OnChange()
{
	GAME_INSTANCE.SetCamera(m_cameraComp);
	m_cameraComp->SetDistance(200.0f);
	m_moveComp->SetActive(true);
}

// 衝突時の押し出し処理
void PlayerCar::CollisionFix(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
{
	Vector3 fix = Vector3::Zero;


	//壁とぶつかったとき
	AABB bgBox = in_hitBox->GetWorldBox();
	AABB playerBox = m_hitBox->GetWorldBox();

	// めり込みを修正
	CalcCollisionFixVec(playerBox, bgBox, fix);

	// 補正ベクトル分戻す
	m_position += fix;

	// 壁衝突時の処理
	if (fix.x > 5.0f || fix.x < -5.0f || fix.y > 5.0f || fix.y < -5.0f)
	{

		// アクセル減少 (衝突時直前の半分のスピードにする)
		m_moveComp->SetAccel(m_moveComp->GetAccelValue() / 2.0f);
	}


	// 位置が変わったのでボックス再計算
	m_hitBox->OnUpdateWorldTransform();

	// printf("[%f, %f, %f]\n", m_position.x, m_position.y, m_position.z);
}
