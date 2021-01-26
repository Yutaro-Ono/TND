//--------------------------------------------------------------+
// ヘリコプタークラス
//--------------------------------------------------------------+
#include "Helicopter.h"
#include "HeliBody.h"
#include "HeliRotorMain.h"
#include "HeliRotorBack.h"
#include "HeliSpotLight.h"
#include "PatrolComponent.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "PointLight.h"
#include "InputController.h"

// 各種パーツのメッシュパス
const std::string HELI_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Body_Internal.OBJ";
const std::string HELI_ROTOR_MAIN_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Main_Rotor_Internal.OBJ";
const std::string HELI_ROTOR_BACK_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Tail_Rotor_Internal.OBJ";
const std::string HELI_SPOTLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Spotlight_Internal.OBJ";

// コンストラクタ
Helicopter::Helicopter(GameWorld* in_world, const Vector3& in_pos, int in_num)
	:m_patrolComp(nullptr)
	,m_state(HELI_STATE::STOP)
	,m_searchSphere(Vector3(in_pos.x, in_pos.y, 0.0f), 1000.0f)
	,m_foundPlayer(false)
	,m_number(in_num)
{
	// ワールドのポインタをセット
	m_world = in_world;

	// 座標を設定
	m_position = in_pos;
	// スケール値は45.0に設定
	SetScale(45.0f);

	// 各種パーツクラスを生成
	m_body = new HeliBody(this, HELI_BODY_MESH_PATH);
	m_mainRotor = new HeliRotorMain(this, HELI_ROTOR_MAIN_MESH_PATH);
	m_backRotor = new HeliRotorBack(this, HELI_ROTOR_BACK_MESH_PATH);
	m_spotLight = new HeliSpotLight(this, HELI_SPOTLIGHT_MESH_PATH);

	// 巡回コンポーネントを生成・追加
	m_patrolComp = new PatrolComponent(this, m_world);


	// ライトカラー (赤)
	Vector3 color = Vector3(1.0f, 0.0f, 0.0f);
	// ポイントライト
	m_pLight[0] = new PointLight(PointLight::VL_MEDIUM);
	m_pLight[0]->SetLightColor(color, color);
	m_pLight[0]->SetPosition(Vector3(m_position.x, m_position.y, 0.0f));
	m_pLight[1] = new PointLight(PointLight::VL_SMALL);
	m_pLight[1]->SetLightColor(color, color);
	m_pLight[1]->SetPosition(Vector3(m_position.x, m_position.y, m_position.z - 10.0f));
	m_pLight[1]->SetFlash(true);
}

// デストラクタ
Helicopter::~Helicopter()
{

}

// 更新処理
void Helicopter::UpdateActor(float in_deltaTime)
{
	// ライト位置
	Vector3 lightPos = Vector3(m_position.x, m_position.y, m_position.z);
	
	if (m_state == HELI_STATE::PATROL || m_state == HELI_STATE::STOP)
	{
		// ヘリの索敵範囲にプレイヤーが接触したかどうかの検出
		SearchPlayer(m_world->GetPlayer());

		// スポットライト位置
		m_pLight[0]->SetPosition(lightPos * Vector3(1.0f, 1.0f, 0.0f));
	}
	else if (m_state == HELI_STATE::CHASE)
	{
		// スポットライト位置がプレイヤーを追跡
		m_pLight[0]->SetPosition(m_world->GetPlayer()->GetPosition() * Vector3(1.0f, 1.0f, 0.0f));
	}

	// ポイントライトの追従処理
	Vector3 trns = Vector3::Transform(Vector3(80.0f, 0.0f, 0.0f), m_rotation);
	m_pLight[1]->SetPosition(lightPos + trns);
}

// ヘリの索敵範囲にプレイヤーが接触したかどうかの検出
void Helicopter::SearchPlayer(PlayerManager* in_player)
{
	// サーチ範囲を更新
	m_searchSphere.m_center.x = m_position.x;
	m_searchSphere.m_center.y = m_position.y;

	// プレイヤーを発見したかどうか
	if (m_searchSphere.Contains(in_player->GetPosition()))
	{
		// プレイヤーが一定以上の速度で走っている時のみ発見状態に
		if (in_player->GetPlayerCar()->GetAccelValue() - in_player->GetPlayerCar()->GetBrakeValue() >= 15.0f)
		{
			m_foundPlayer = true;
			printf("ヘリがプレイヤーを発見した\n");
		}

	}
	else
	{
		m_foundPlayer = false;
	}
}
