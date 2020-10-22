#include "ClientActor.h"
#include "SpriteComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "Texture.h"

const float AnimationSpeed = 0.5f;        // アニメーションの速度

ClientActor::ClientActor(const Vector3& in_pos)
	:m_isSelected(false)
	,m_landMark(nullptr)
	,m_animState(CLIENT_ANIM::ANIM_IDLE_LOOKAROUND)
{
	// 座標設定
	m_position = in_pos;
	// スケール設定
	SetScale(0.3f);

	// テクスチャ生成
	Texture* texture = new Texture();
	texture->Load("Data/Interface/landmark.png");
	m_landMark = new SpriteComponent(this);
	m_landMark->SetTexture(texture);

	// 依頼人のメッシュ生成
	Mesh* mesh = RENDERER->GetMesh("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003_ue4.gpmesh");
	m_skelComp = new SkeletalMeshComponent(this);
	m_skelComp->SetMesh(mesh);
	m_skelComp->SetSkeleton(RENDERER->GetSkeleton("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003_ue4.gpskel"));

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

ClientActor::~ClientActor()
{
}

void ClientActor::UpdateActor(float in_deltaTime)
{
	m_recomputeWorldTransform = true;
}
