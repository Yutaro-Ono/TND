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
#include "CarSoundComponent.h"

const std::string PlayerCar::CAR_HANDLE_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Handle/SM_suv_steering_wheel_lod0_Internal.OBJ";

// コンストラクタ
PlayerCar::PlayerCar()
	:m_isActive(true)
	,m_driveState(DRIVE_IDLE)
	,m_turnState(TURN_IDLE)
	,m_terrainType(LevelTerrain::TYPE_STREET)
	,m_friction(1.0f)
	,m_hitBox(nullptr)
	,m_hitGroundBox(nullptr)
	,m_hitHeadBox(nullptr)
{
	// 車両操作用のMoveComponentを生成
	m_moveComp = new MoveComponentCar(this);
	m_moveComp->SetActive(false);
	// カメラコンポーネントを生成
	m_cameraComp = new ThirdPersonCarCamera(this);
	m_cameraComp->SetChaseOwnerForward(false);
	m_cameraComp->SetDistance(200.0f);
	// サウンドコンポーネントを生成
	m_soundComp = new CarSoundComponent(this);

	// 各パーツごとのインスタンスを作成
	m_body = new CarBody(this);
	m_door[0] = new CarDoor(this, CarDoor::DOOR_POS::LEFT);
	m_door[1] = new CarDoor(this, CarDoor::DOOR_POS::RIGHT);
	m_handle = new CarHandle(this, CAR_HANDLE_MESH_PATH);
	// タイヤ
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

	// プレイヤーが車を運転している状態の時
	if (m_manager->GetPlayerMode() == PlayerManager::PLAYER_MODE::MODE_CAR)
	{

	}
	else
	{
		// 移動・効果音再生コンポーネント停止
		m_moveComp->SetActive(false);
		m_soundComp->SetStop(true);
		m_soundComp->StopEngineSound();
		// タイヤの回転を停止
		for (int i = 0; i < 4; i++)
		{
			m_wheel[i]->SetSpin(false);
		}
	}



}


// 人間操作から切り替わった時の各種変更処理
void PlayerCar::OnChange()
{
	// カメラを車専用のものにセット
	GAME_INSTANCE.SetCamera(m_cameraComp);
	m_cameraComp->SetDistance(200.0f);
	// 移動コンポーネントの有効化
	m_moveComp->SetActive(true);
	// 効果音再生コンポーネントの再開・開始エンジン音の再生
	m_soundComp->SetStop(false);
	m_soundComp->PlayEngineStart();
	// タイヤの回転有効化
	for (int i = 0; i < 4; i++)
	{
		m_wheel[i]->SetSpin(true);
	}
}

// 衝突時の押し出し処理
void PlayerCar::CollisionFix(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
{
	Vector3 fix = Vector3::Zero;

	//壁とぶつかったとき
	AABB bgBox = in_hitBox->GetWorldBox();
	AABB playerBox = m_hitBox->GetWorldBox();


	// 床の当たり判定ボックスなら
	if (in_hitBox->GetPhysicsType() == PhysicsWorld::TYPE_TERRAIN)
	{
		if (bgBox.Contains(m_position - Vector3(0.0f, 0.0f, 10.0f)))
		{
			// 現在の地形タイプを格納
			SetOnTerrainType(in_hitBox->GetTerrainPtr()->GetNodeType());
			// 地形ごとに摩擦力を設定
			if (m_terrainType == LevelTerrain::TYPE_GLASS)
			{
				m_friction = in_hitBox->GetTerrainPtr()->GetFrictionVal();
			}
			if (m_terrainType == LevelTerrain::TYPE_STREET)
			{
				m_friction = in_hitBox->GetTerrainPtr()->GetFrictionVal();
			}
		}

	}
	// 建物にぶつかった時
	else if (in_hitBox->GetPhysicsType() == PhysicsWorld::TYPE_BACK_GROUND)
	{
		// 効果音再生 (速度に応じて効果音の大きさを調整)
		float accel = m_moveComp->GetAccelValue();
		if (accel > 10.0f && accel < 65.0f)
		{
			m_soundComp->PlayCrushMed();
		}
		else if (accel >= 65.0f)
		{
			m_soundComp->PlayCrushHard();
		}
	}

	// めり込みを修正
	CalcCollisionFixVec(playerBox, bgBox, fix);

	// 補正ベクトル分戻す
	m_position += fix;

	// 衝突時の処理
	if (fix.x > 5.0f || fix.x < -5.0f || fix.y > 5.0f || fix.y < -5.0f)
	{
		// アクセル減少 (衝突時直前の半分のスピードにする)
		m_moveComp->SetAccel(m_moveComp->GetAccelValue() / 2.0f);
	}


	// 位置が変わったのでボックス再計算
	m_hitBox->OnUpdateWorldTransform();

	// printf("[%f, %f, %f]\n", m_position.x, m_position.y, m_position.z);
}

