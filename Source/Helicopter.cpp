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

// 各種パーツのメッシュパス
const std::string HELI_BODY_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Body_Internal.OBJ";
const std::string HELI_ROTOR_MAIN_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Main_Rotor_Internal.OBJ";
const std::string HELI_ROTOR_BACK_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Tail_Rotor_Internal.OBJ";
const std::string HELI_SPOTLIGHT_MESH_PATH = "Data/Meshes/TND/Actors/Police/Helicopter/Helicopter_Spotlight_Internal.OBJ";

// コンストラクタ
Helicopter::Helicopter(GameWorld* in_world, const Vector3& in_pos)
	:m_patrolComp(nullptr)
	,m_searchSphere(Vector3(in_pos.x, in_pos.y, 0.0f), 100.0f)
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
	AddComponent(m_patrolComp);


	
}

// デストラクタ
Helicopter::~Helicopter()
{
}

// 更新処理
void Helicopter::UpdateActor(float in_deltaTime)
{
	// サーチ範囲を更新
	m_searchSphere.m_center.x = m_position.x;
	m_searchSphere.m_center.y = m_position.y;


	if (m_searchSphere.Contains(m_world->GetPlayer()->GetPosition()))
	{
		printf("ヘリとプレイヤー当たった！\n");
	}

}
