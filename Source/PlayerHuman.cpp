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
	// �J�����̐���
	m_camera = new HumanCamera(this);

	// �X�P���^�����b�V���̃��[�h
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003.gpmesh");
	m_skelMeshComp = new SkeletalMeshComponent(this);
	m_skelMeshComp->SetMesh(mesh);
	m_skelMeshComp->SetSkeleton(RENDERER->GetSkeleton("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003.gpskel"));

	// �A�j���[�V�����擾
	m_animTypes.resize(PLAYER_ANIM::ANIM_ALL_NUM);
	// �Ή�����A�j���[�V�����z��ɃZ�b�g
	m_animTypes[ANIM_IDLE] = RENDERER->GetAnimation("Data/Animation/ThirdPersonIdle.gpanim", true);
	m_animTypes[ANIM_WALKING] = RENDERER->GetAnimation("Data/Animation/ThirdPersonRun.gpanim", true);
	m_animTypes[ANIM_RUNNING] = RENDERER->GetAnimation("Data/Animation/ThirdPersonRun.gpanim", true);
	m_animTypes[ANIM_JUMPSTART] = RENDERER->GetAnimation("Data/Animation/ThirdPersonJump_Start.gpanim", true);
	m_animTypes[ANIM_JUMPLOOP] = RENDERER->GetAnimation("Data/Animation/ThirdPersonJump_Loop.gpanim", true);
	m_animTypes[ANIM_LANDING] = RENDERER->GetAnimation("Data/Animation/ThirdPersonJump_End.gpanim", true);

	// �A�C�h�����̃A�j���[�V�������Z�b�g���Ă���
	m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);

	// �����蔻��{�b�N�X�̃Z�b�g
	AABB playerBox = mesh->GetCollisionBox();
	m_hitBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	playerBox.m_min.x *= 0.6f;
	playerBox.m_min.y *= 0.6f;
	playerBox.m_max.x *= 0.6f;
	playerBox.m_max.y *= 0.6f;
	m_hitBox->SetObjectBox(playerBox);

	// �v���[���[�̑����𒲂ׂ�{�b�N�X���쐬�@�{�b�N�X��1/4, �{�b�N�X��ʂ����_�ɗ���悤�ɂ���
	AABB groundBox;
	groundBox = playerBox;
	groundBox.m_min.x *= 0.8f;
	groundBox.m_min.y *= 0.8f;
	groundBox.m_max.x *= 0.8f;
	groundBox.m_max.y *= 0.8f;
	groundBox.m_min.z = -2.0f;  //�W�����v���Ɉ���������Ȃ�����
	groundBox.m_max.z *= 0.0f;
	m_hitGroundBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER);
	m_hitGroundBox->SetObjectBox(groundBox);

	// �v���[���[�̓���𒲂ׂ�{�b�N�X���쐬 �{�b�N�X��ʂ�����ɗ���悤�ɂ���
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
		// �A�C�h�����̃A�j���[�V�������Z�b�g
		m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);
	}
}

void PlayerHuman::ChangeState()
{
}

void PlayerHuman::FixCollision(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
{
}
