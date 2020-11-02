#include "ClientActor.h"
#include "WorldSpaceUI.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"
#include "MissionBase.h"


const std::string MESH_PATH_CARLA = "Data/Meshes/TND/Actors/Player/carla/rp_carla_rigged_001_ue4";

const float AnimationSpeed = 0.5f;        // アニメーションの速度

// コンストラクタ
ClientActor::ClientActor(const Vector3& in_pos)
	:m_isSelected(false)
	,m_isAccepted(false)
	,m_setting(CLIENT_SETTING::NONE)
	,m_landMark(nullptr)
	,m_animState(CLIENT_ANIM::ANIM_IDLE_LOOKAROUND)
	,m_hitBox(nullptr)
{
	// 座標設定
	m_position = in_pos;
	// スケール設定
	SetScale(0.265f);

	// ランドマーク生成
	m_landMark = new WorldSpaceUI(m_position, "Data/Interface/landmark.png", 300.0f);


	// 依頼人のメッシュ生成
	Mesh* mesh = RENDERER->GetMesh(MESH_PATH_CARLA + ".gpmesh");
	m_skelComp = new SkeletalMeshComponent(this);
	m_skelComp->SetMesh(mesh);
	m_skelComp->SetSkeleton(RENDERER->GetSkeleton(MESH_PATH_CARLA + ".gpskel"));

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


}

// デストラクタ
ClientActor::~ClientActor()
{
	delete m_landMark;
}

// 更新処理
void ClientActor::UpdateActor(float in_deltaTime)
{
	// この依頼人がミッションに設定されている時のみランドマーク表示
	if (m_isSelected && (m_setting == CLIENT_SETTING::START || m_setting == CLIENT_SETTING::GOAL))
	{
		m_landMark->SetVisible(true);
	}
	else
	{
		m_landMark->SetVisible(false);
	}

	//if (m_setting == CLIENT_SETTING::NONE)
	//{
	//	m_landMark->SetVisible(false);
	//}
	//else
	//{

	//}



	//


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
