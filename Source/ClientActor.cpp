#include "ClientActor.h"
#include "WorldSpaceUI.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"
#include "MissionBase.h"
#include "Font.h"
#include "PointLight.h"
#include <string>
#include <sstream>
#include <random>

// キャラごとのメッシュパス
const std::string MESH_PATH_CARLA = "Data/Meshes/TND/Actors/Player/carla/rp_carla_rigged_001_ue4";
const std::string MESH_PATH_CLAUDIA = "Data/Meshes/TND/Actors/Player/claudia/rp_claudia_rigged_002_ue4";
const std::string MESH_PATH_ERIC = "Data/Meshes/TND/Actors/Player/eric/rp_eric_rigged_001_ue4";
const std::string MESH_PATH_MANUEL = "Data/Meshes/TND/Actors/Player/manuel/rp_manuel_rigged_001_ue4";
const std::string MESH_PATH_SOPHIA = "Data/Meshes/TND/Actors/Player/sophia/rp_sophia_rigged_003_ue4";
const std::string MESH_PATH_NATHAN = "Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003_ue4";

const float AnimationSpeed = 0.5f;        // アニメーションの速度

// コンストラクタ
ClientActor::ClientActor(const Vector3& in_pos, int in_chara)
	:m_isSelected(false)
	,m_isAccepted(false)
	,m_distancePlayer(0.0f)
	,m_setting(CLIENT_SETTING::NONE)
	,m_landMark(nullptr)
	,m_distanceTex(nullptr)
	,m_distanceWorld(nullptr)
	,m_animState(CLIENT_ANIM::ANIM_IDLE_LOOKAROUND)
	,m_hitBox(nullptr)
	,m_light(nullptr)
{
	// 座標設定
	m_position = in_pos;
	// スケール設定
	SetScale(0.265f);

	// ランドマーク生成
	m_landMark = new WorldSpaceUI(m_position + Vector3(0.0f, 0.0f, -30.0f), "Data/Interface/TND/Control/landMark.png", 200.0f);
	m_landMark->SetVisible(false);

	// 距離テクスチャ生成
	std::stringstream distStream;
	distStream << (double)m_distancePlayer << "m";
	m_distanceTex = GAME_INSTANCE.GetFont(GAME_INSTANCE.GetFontPath())->RenderText(distStream.str());
	m_distanceWorld = new WorldSpaceUI(m_position + Vector3(0.0f, 30.0f, -30.0f), m_distanceTex, 200.0f);
	m_distanceWorld->SetVisible(false);

	// 依頼人のメッシュ生成
	LoadMeshEachChara(in_chara);

	// アニメーションの生成・取得
	m_anim.resize(CLIENT_ANIM::ANIM_ALL_NUM);
	// 対応するアニメーション配列にセット
	m_anim[ANIM_IDLE] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle.gpanim", true);
	m_anim[ANIM_IDLE_LOOKAROUND] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle_Around.gpanim", true);
	m_anim[ANIM_WALKING_FWD] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Fwd.gpanim", true);
	m_anim[ANIM_WALKING_BWD] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Bwd.gpanim", true);
	m_anim[ANIM_WALKING_LEFT] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Left.gpanim", true);
	m_anim[ANIM_WALKING_RIGHT] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Right.gpanim", true);
	m_anim[ANIM_RUNNING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonRun.gpanim", true);
	m_anim[ANIM_JUMPSTART] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Start.gpanim", false);
	m_anim[ANIM_JUMPLOOP] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Loop.gpanim", true);
	m_anim[ANIM_LANDING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_End.gpanim", false);

	// アイドル時のアニメーションをセット
	m_skelComp->PlayAnimation(m_anim[ANIM_IDLE_LOOKAROUND], AnimationSpeed);

	// ポイントライト
	m_light = new PointLight(PointLight::VL_MEDIUM);
	m_light->SetLuminance(10.0f);
}

// デストラクタ
ClientActor::~ClientActor()
{
	delete m_landMark;
	delete m_distanceWorld;
}

// 更新処理
void ClientActor::UpdateActor(float in_deltaTime)
{

	// この依頼人がミッションに設定されている時のみランドマーク表示
	if (m_isSelected && (m_setting == CLIENT_SETTING::START || m_setting == CLIENT_SETTING::GOAL))
	{
		m_landMark->SetVisible(true);
		m_distanceWorld->SetVisible(true);

		// プレイヤーとの距離に応じたランドマークのスケール変更
		float scale = m_distancePlayer / 15.0f;
		if (scale < 30.0f)
		{
			scale = 30.0f;
		}
		if (scale > 2000.0f)
		{
			scale = 2000.0f;
		}
		// 距離更新
		if (m_distanceTex != nullptr) { m_distanceTex->Delete(); }
		std::stringstream distStream;
		
		distStream << (long)m_distancePlayer / 20 << "m";
		m_distanceTex = GAME_INSTANCE.GetFont(GAME_INSTANCE.GetFontPath())->RenderText(distStream.str(), Vector3(1.0f, 1.0f, 1.0f), 72);
		m_distanceWorld->SetTexture(m_distanceTex);
		m_distanceWorld->SetPosition(m_position + Vector3(0.0f, 0.0f, scale / 10.0f * 10.0f));
		if (scale <= 300.0f)
		{
			m_distanceWorld->SetVisible(false);
		}



		m_landMark->SetScale(scale);
		m_distanceWorld->SetScale(scale);
	}
	else
	{
		m_landMark->SetVisible(false);
		m_distanceWorld->SetVisible(false);
	}

	m_light->SetPosition(m_position);

	m_recomputeWorldTransform = true;
}


// プレイヤーマネージャークラスで呼び出される関数
// 
void ClientActor::SetAcceptedPlayer()
{
	// 設定されたミッションが開始前の状態且つ自身が開始NPCならtrue
	if (m_mission->GetMissionState() == MissionBase::HOLD && m_setting == CLIENT_SETTING::START)
	{
		m_isAccepted = true;
		return;
	}

	// 設定されたミッションがアクティブな状態且つ自身が終了NPCならtrue
	if (m_mission->GetMissionState() == MissionBase::ACTIVE && m_setting == CLIENT_SETTING::GOAL)
	{
		m_isAccepted = true;
		return;
	}
}

// コンストラクタで指定された引数に応じたメッシュのロード
void ClientActor::LoadMeshEachChara(int in_chara)
{
	int chara = in_chara;
	std::string meshPath;
	std::string skelPath;

	if (chara < 0 || chara > 5)
	{
		chara = std::rand() % 4;
	}

	// CARLA(黒人女性)
	if (chara == 0)
	{
		meshPath = MESH_PATH_CARLA + ".gpmesh";
		skelPath = MESH_PATH_CARLA + ".gpskel";
	}
	
	// CLAUDIA(白人女性)
	if (chara == 1)
	{
		meshPath = MESH_PATH_CLAUDIA + ".gpmesh";
		skelPath = MESH_PATH_CLAUDIA + ".gpskel";
	}
	// ERIC(白人男性)
	if (chara == 2)
	{
		meshPath = MESH_PATH_ERIC + ".gpmesh";
		skelPath = MESH_PATH_ERIC + ".gpskel";
	}
	// MANUEL(白人男性)
	if (chara == 3)
	{
		meshPath = MESH_PATH_ERIC + ".gpmesh";
		skelPath = MESH_PATH_ERIC + ".gpskel";
	}
	// SOPHIA(白人女性)
	if (chara == 4)
	{
		meshPath = MESH_PATH_SOPHIA + ".gpmesh";
		skelPath = MESH_PATH_SOPHIA + ".gpskel";
	}

	if (chara == 5)
	{
		meshPath = MESH_PATH_NATHAN + ".gpmesh";
		skelPath = MESH_PATH_NATHAN + ".gpskel";
	}

	Mesh* mesh = RENDERER->GetMesh(meshPath);
	m_skelComp = new SkeletalMeshComponent(this);
	m_skelComp->SetMesh(mesh);
	m_skelComp->SetSkeleton(RENDERER->GetSkeleton(skelPath));
}
