#include "PlayerHuman.h"
#include "PlayerManager.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "HumanCamera.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"

const float cAnimationSpeed = 0.5f;

PlayerHuman::PlayerHuman(class PlayerManager* in_manager)
	:m_manager(in_manager)
	,m_jumpVec(Vector3::Zero)
{
	// カメラの生成
	m_camera = new HumanCamera(this);

	// スケルタルメッシュのロード
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003.gpmesh");
	m_skelMeshComp = new SkeletalMeshComponent(this);
	m_skelMeshComp->SetMesh(mesh);
	m_skelMeshComp->SetSkeleton(RENDERER->GetSkeleton("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003.gpskel"));

	// アニメーション取得
	m_animTypes.resize(PLAYER_ANIM::ANIM_ALL_NUM);
	// 対応するアニメーション配列にセット
	m_animTypes[ANIM_IDLE] = RENDERER->GetAnimation("Data/Animation/ThirdPersonIdle.gpanim", true);
	m_animTypes[ANIM_WALKING] = RENDERER->GetAnimation("Data/Animation/ThirdPersonRun.gpanim", true);
	m_animTypes[ANIM_RUNNING] = RENDERER->GetAnimation("Data/Animation/ThirdPersonRun.gpanim", true);
	m_animTypes[ANIM_JUMPSTART] = RENDERER->GetAnimation("Data/Animation/ThirdPersonJump_Start.gpanim", true);
	m_animTypes[ANIM_JUMPLOOP] = RENDERER->GetAnimation("Data/Animation/ThirdPersonJump_Loop.gpanim", true);
	m_animTypes[ANIM_LANDING] = RENDERER->GetAnimation("Data/Animation/ThirdPersonJump_End.gpanim", true);

	// アイドル時のアニメーションをセットしておく
	m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);

	// 当たり判定ボックスのセット
	AABB playerBox = mesh->GetCollisionBox();
	m_hitBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	playerBox.m_min.x *= 0.6f;
	playerBox.m_min.y *= 0.6f;
	playerBox.m_max.x *= 0.6f;
	playerBox.m_max.y *= 0.6f;
	m_hitBox->SetObjectBox(playerBox);

	// プレーヤーの足元を調べるボックスを作成　ボックス高1/4, ボックス上面が原点に来るようにする
	AABB groundBox;
	groundBox = playerBox;
	groundBox.m_min.x *= 0.8f;
	groundBox.m_min.y *= 0.8f;
	groundBox.m_max.x *= 0.8f;
	groundBox.m_max.y *= 0.8f;
	groundBox.m_min.z = -2.0f;  //ジャンプ時に引っかからない高さ
	groundBox.m_max.z *= 0.0f;
	m_hitGroundBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	m_hitGroundBox->SetObjectBox(groundBox);

	// プレーヤーの頭上を調べるボックスを作成 ボックス底面が頭上に来るようにする
	AABB headBox;
	headBox = groundBox;
	headBox.m_min.z = playerBox.m_max.z;
	headBox.m_max.z = headBox.m_min.z + 2.0f;
	m_hitHeadBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	m_hitHeadBox->SetObjectBox(headBox);

}

PlayerHuman::~PlayerHuman()
{
	m_animTypes.clear();
}

void PlayerHuman::UpdateActor(float in_deltaTime)
{
	if (m_manager->GetPlayerMode() == PlayerManager::MODE_HUMAN)
	{
		
	}

	else
	{
		// アイドル時のアニメーションをセット
		m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);
	}
}

void PlayerHuman::ChangeState()
{
}

void PlayerHuman::FixCollision(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
{
}
