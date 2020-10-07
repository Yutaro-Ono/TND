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
#include "Input.h"
#include "InputController.h"

const float cAnimationSpeed = 0.5f;

PlayerHuman::PlayerHuman(class PlayerManager* in_manager)
	:m_manager(in_manager)
	,m_jumpVec(Vector3(0.0f, 0.0f, 0.0f))
{


	// �J�����̐���
	m_camera = new HumanCamera(this);

	// �X�P���^�����b�V���̃��[�h
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Actors/Player/rp_eric_rigged_001_ue4.gpmesh");
	m_skelMeshComp = new SkeletalMeshComponent(this);
	m_skelMeshComp->SetMesh(mesh);
	m_skelMeshComp->SetSkeleton(RENDERER->GetSkeleton("Data/Meshes/TND/Actors/Player/rp_eric_rigged_001_ue4.gpskel"));

	// �A�j���[�V�����擾
	m_animTypes.resize(PLAYER_ANIM::ANIM_ALL_NUM);
	// �Ή�����A�j���[�V�����z��ɃZ�b�g
	m_animTypes[ANIM_IDLE] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle.gpanim", true);
	m_animTypes[ANIM_WALKING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle.gpanim", true);
	m_animTypes[ANIM_RUNNING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonRun.gpanim", true);
	m_animTypes[ANIM_JUMPSTART] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Start.gpanim", true);
	m_animTypes[ANIM_JUMPLOOP] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Loop.gpanim", true);
	m_animTypes[ANIM_LANDING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_End.gpanim", true);

	// �A�C�h�����̃A�j���[�V�������Z�b�g���Ă���
	m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed);


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
		//�L������ԑJ��
		ChangeState();

		// �W�����v�I�����̓L�[���͎󂯕t���Ȃ�
		if (m_animState == ANIM_LANDING)
		{
			return;
		}

		//�L��������
		const float speed = 200.0f;
		Vector3 forwardVec;
		Vector3 rightVec = Vector3(1.0f, 0.0f, 0.0f);
		Vector3 charaForwardVec = GetForward();

		// �L�����N�^�[�ړ�
		Vector3 DirVec(0.0f, 0.0f, 0.0f);

		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_RIGHT))
		{
			DirVec = rightVec;
		}

		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LEFT))
		{
			DirVec = -1.0f * rightVec;
		}

		//�W�����v
		if (INPUT_INSTANCE.IsKeyPushDown(SDL_SCANCODE_SPACE) && !m_isJump)
		{
			m_jumpVec.z += 320.0f * in_deltaTime;
			m_isJump = true;
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPSTART], cAnimationSpeed);
			m_animState = ANIM_JUMPSTART;
		}

		if (m_isJump)
		{
			m_jumpVec.z -= 1.0f * in_deltaTime;
		}

		if (DirVec.LengthSq() > 0.5f)
		{
			// �����L�[����
			charaForwardVec = DirVec;

			// �i�s�����Ɍ����ĉ�]
			charaForwardVec.Normalize();
			RotateToNewForward(charaForwardVec);

			// ���݂̃X�s�[�h��ۑ�
			m_speed = speed * in_deltaTime;
		}
		else
		{
			charaForwardVec = Vector3(0, 0, 0);
		}

		// �i�s�����Ɉړ�
		m_position += m_speed * charaForwardVec + m_jumpVec;
		m_recomputeWorldTransform = true;

		// �����蔻�菈���Œn�ʃt���O��false�ɂ��A���̌�̓����蔻�菈����true�ɂȂ邩�`�F�b�N����
		m_onGround = false;
	}

	else
	{
		// �A�C�h�����̃A�j���[�V�������Z�b�g
		//m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);
	}
}

void PlayerHuman::ChangeState()
{
	//�O��̓����蔻�菈���ŉ��ɒn�ʂ��Ȃ�������W�����v���ֈڍs����
	if (!m_isJump && !m_onGround && m_position.z > 0.0)
	{
		m_isJump = true;
		if (m_animState != ANIM_JUMPLOOP)
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPLOOP], cAnimationSpeed);
			m_animState = ANIM_JUMPLOOP;
		}
		return;
	}
	if (m_isJump)
	{
		// JUMP�J�n����JumpLoop��
		if (m_animState == ANIM_JUMPSTART)
		{
			if (!(m_skelMeshComp->IsPlaying()))
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPLOOP], cAnimationSpeed);
				m_animState = ANIM_JUMPLOOP;
			}
		}
		return;
	}

	// �L�[���͂���A�j���[�V������Ԃֈڍs
	bool IsIdleStart = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_UP) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_DOWN) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_LEFT);

	// �ҋ@�A�j���J�n
	if (IsIdleStart)
	{
		if (m_animState == ANIM_LANDING)
		{
			if (!m_skelMeshComp->IsPlaying())
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);
				m_animState = ANIM_IDLE;
			}
		}
		else if ((m_animState != ANIM_IDLE))
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed);
			m_animState = ANIM_IDLE;
		}
	}
	else
	{
		// �W�����v�I���A�j�������RUN�J�n
		if (m_animState == ANIM_LANDING)
		{
			if (!m_skelMeshComp->IsPlaying())
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed);
				m_animState = ANIM_RUNNING;
			}
		}

		// RUN�A�j���J�n
		if (m_animState != ANIM_RUNNING)
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed);
			m_animState = ANIM_RUNNING;
		}
	}
}

void PlayerHuman::CollisionFix(BoxCollider* in_hitPlayerBox, BoxCollider* in_hitBox)
{
	Vector3 fix;
	//���������蔻�肪�q�b�g�������H
	if (in_hitPlayerBox == m_hitGroundBox && m_jumpVec.z <= 0.0f)
	{
		m_onGround = true;
		// �W�����v���Ȃ����
		if (m_isJump)
		{
			m_isJump = false;
			m_jumpVec = Vector3(0, 0, -1.0f);
			m_position += m_jumpVec;
			if (m_animState == ANIM_JUMPLOOP)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_LANDING], cAnimationSpeed);
				m_animState = ANIM_LANDING;
			}
			return;
		}
	}

	//�ǂƂԂ������Ƃ�
	AABB bgBox = in_hitBox->GetWorldBox();
	AABB playerBox = m_hitBox->GetWorldBox();

	// �߂荞�݂��C��
	CalcCollisionFixVec(playerBox, bgBox, fix);

	// �␳�x�N�g�����߂�
	m_position += fix;

	// �W�����v�A�N�V�������ɓ����ǂɓ��������Ƃ�
	if (in_hitPlayerBox == m_hitHeadBox && m_isJump)
	{
		//�W�����v������ɍs���������t�]������
		m_jumpVec = Vector3(0, 0, -1.0f);
		m_position += m_jumpVec; // ��������ɓ�����Ȃ��l�ɗ���
	}

	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	m_hitBox->OnUpdateWorldTransform();
}
