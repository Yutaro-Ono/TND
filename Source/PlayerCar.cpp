#include "PlayerCar.h"
#include "MoveComponentCar.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "PhysicsWorld.h"

const std::string PlayerCar::CAR_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Body/SM_suv_parts_LOD0_body_Internal.OBJ";
const std::string PlayerCar::CAR_DOOR_LEFT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/SM_suv_parts_LOD0_left_door_Internal.OBJ";
const std::string PlayerCar::CAR_DOOR_RIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Door/SM_suv_parts_LOD0_right_door_Internal.OBJ";
const std::string PlayerCar::CAR_WHEEL_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Wheel/SM_suv_parts_LOD0_wheel_Internal.OBJ";
const std::string PlayerCar::CAR_HANDLE_MESH_PATH = "Data/Meshes/TND/Actors/Car/Player/Handle/SM_suv_steering_wheel_lod0_Internal.OBJ";

// コンストラクタ
PlayerCar::PlayerCar()
	:m_isActive(false)
{
	// 車両操作用のMoveComponentを生成
	m_moveComp = new MoveComponentCar(this);

	// 各パーツごとのクラスを作成
	m_body = new CarBody(this, CAR_BODY_MESH_PATH);
	m_door[0] = new CarDoor(this, CAR_DOOR_LEFT_MESH_PATH);
	m_door[1] = new CarDoor(this, CAR_DOOR_RIGHT_MESH_PATH);
	m_handle = new CarHandle(this, CAR_HANDLE_MESH_PATH);
	for (int i = 0; i < 4; i++)
	{
		m_wheel[i] = new CarWheel(this, CAR_WHEEL_MESH_PATH);
	}
}

PlayerCar::~PlayerCar()
{
}

void PlayerCar::UpdateActor(float in_deltaTime)
{

	bool u = true;

	if (!u)
	{
		int a = 0;
	}

	// ディアクティベートされたら
	if (!m_isActive)
	{
		// メッシュの表示を切らせるよう命令
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

// 受け取ったメッシュから当たり判定ボックスを生成
void PlayerCar::CreateAABB(Mesh* in_mesh)
{
	// 当たり判定セット
	AABB playerBox = in_mesh->GetCollisionBox();
	m_hitBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	playerBox.m_min.x *= 0.6f;
	playerBox.m_min.y *= 0.6f;
	playerBox.m_max.x *= 0.6f;
	playerBox.m_max.y *= 0.6f;
	m_hitBox->SetObjectBox(playerBox);
}

// 衝突時の押し出し処理
void PlayerCar::CollisionFix(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
{
	Vector3 fix;


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
