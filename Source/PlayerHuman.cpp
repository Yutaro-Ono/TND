#include "PlayerHuman.h"
#include "PlayerManager.h"
#include "GameMain.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "HumanCamera.h"
#include "ThirdPersonCamera.h"
#include "MoveComponentHuman.h"
#include "PhysicsWorld.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Input.h"
#include "InputController.h"
#include "PointLight.h"

const float cAnimationSpeed = 14.0f;          // �A�j���[�V�����̑��x

PlayerHuman::PlayerHuman(class PlayerManager* in_manager)
	:m_manager(in_manager)
	,m_pov(POV_THIRD_PERSON)
	,m_jumpVec(Vector3(0.0f, 0.0f, 0.0f))
	,m_faceLight(nullptr)
	,m_isActive(true)
{
	// �J�����̐���
	m_cameraComp = new ThirdPersonCamera(this);
	m_cameraComp->SetAdjustForward(true);
	m_cameraComp->SetChaseOwnerForward(false);

	// MoveComponent�̐���
	m_moveComp = new MoveComponentHuman(this);
	
	// �X�P���^�����b�V���̃��[�h
	Mesh* mesh = RENDERER->GetMesh("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003_ue4.gpmesh");
	m_skelMeshComp = new SkeletalMeshComponent(this);
	m_skelMeshComp->SetMesh(mesh);
	m_skelMeshComp->SetSkeleton(RENDERER->GetSkeleton("Data/Meshes/TND/Actors/Player/rp_nathan_rigged_003_ue4.gpskel"));

	// �t�F�C�X���C�g
	m_faceLight = new PointLight(PointLight::VL_SMALL);
	m_faceLight->SetPosition(m_position + Vector3(0.0f, 0.0f, 32.0f));
	m_faceLight->SetLightColor(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f));
	m_faceLight->SetLuminance(1.0f);
	// �A�j���[�V�����擾
	m_animTypes.resize(PLAYER_ANIM::ANIM_ALL_NUM);
	// �Ή�����A�j���[�V�����z��ɃZ�b�g
	m_animTypes[ANIM_IDLE] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle.gpanim", true);
	m_animTypes[ANIM_IDLE_LOOKAROUND] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonIdle_Around.gpanim", true);
	m_animTypes[ANIM_WALKING_FWD] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Fwd.gpanim", true);
	m_animTypes[ANIM_WALKING_BWD] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Bwd.gpanim", true);
	m_animTypes[ANIM_WALKING_LEFT] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Left.gpanim", true);
	m_animTypes[ANIM_WALKING_RIGHT] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonWalk_Right.gpanim", true);
	m_animTypes[ANIM_RUNNING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonRun.gpanim", true);
	m_animTypes[ANIM_JUMPSTART] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Start.gpanim", false);
	m_animTypes[ANIM_JUMPLOOP] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_Loop.gpanim", true);
	m_animTypes[ANIM_LANDING] = RENDERER->GetAnimation("Data/Animation/Player/ThirdPersonJump_End.gpanim", false);

	// �A�C�h�����̃A�j���[�V�������Z�b�g���Ă���
	m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed * GAME_INSTANCE.GetDeltaTime());

	// �����蔻��{�b�N�X�̃Z�b�g
	AABB playerBox = mesh->GetCollisionBox();
	m_hitBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER_HUMAN);
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
	m_hitGroundBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER_HUMAN);
	m_hitGroundBox->SetObjectBox(groundBox);

	// �v���[���[�̓���𒲂ׂ�{�b�N�X���쐬 �{�b�N�X��ʂ�����ɗ���悤�ɂ���
	AABB headBox;
	headBox = groundBox;
	headBox.m_min.z = playerBox.m_max.z;
	headBox.m_max.z = headBox.m_min.z + 2.0f;
	m_hitHeadBox = new BoxCollider(this, PhysicsWorld::TYPE_PLAYER_HUMAN);
	m_hitHeadBox->SetObjectBox(headBox);

}

PlayerHuman::~PlayerHuman()
{
	m_animTypes.clear();
}

void PlayerHuman::UpdateActor(float in_deltaTime)
{

	// �v���C���[���쎞�̍X�V����
	if (m_manager->GetPlayerMode() == PlayerManager::MODE_HUMAN)
	{
		// �J�������A�N�e�B�u�ɂ���
		GAME_INSTANCE.SetCamera(m_cameraComp);
		m_moveComp->SetActive(true);
		m_skelMeshComp->SetVisible(true);

		// �d�͏���
		//m_position.z -= 1.0f;
		if (m_position.z < -20.0f)
		{
			m_position.z = 0.0f;
		}

		//�L������ԑJ��
		ChangeState(in_deltaTime);

		// �W�����v�I�����̓L�[���͎󂯕t���Ȃ�
		if (m_animState == ANIM_LANDING)
		{
			return;
		}

		m_recomputeWorldTransform = true;

		// �����蔻�菈���Œn�ʃt���O��false�ɂ��A���̌�̓����蔻�菈����true�ɂȂ邩�`�F�b�N����
		m_onGround = false;
	}

	else
	{
		m_moveComp->SetActive(false);
		m_skelMeshComp->SetVisible(false);
		// �A�C�h�����̃A�j���[�V�������Z�b�g
		m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed * in_deltaTime);
	}

	// ���C�g�̒Ǐ]
	m_faceLight->SetPosition(m_position + Vector3(0.0f, 0.0f, 32.0f));

	printf("Player x : %f, y : %f, z : %f\n", m_position.x, m_position.y, m_position.z);
}

void PlayerHuman::ChangeState(float in_deltaTime)
{
	//�O��̓����蔻�菈���ŉ��ɒn�ʂ��Ȃ�������W�����v���ֈڍs����
	if (!m_isJump && !m_onGround && m_position.z > 0.0)
	{
		m_isJump = true;
		if (m_animState != ANIM_JUMPLOOP)
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPLOOP], cAnimationSpeed * in_deltaTime);
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
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_JUMPLOOP], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_JUMPLOOP;
			}
		}
		return;
	}

	// �ړ��R�}���h�̓��͒l�擾
	Vector2 axisL = Vector2::Zero;

	if (CONTROLLER_INSTANCE.IsAvailable())
	{
		// �R���g���[���ڑ����A���X�e�B�b�N�̓��͒l���擾
		axisL = CONTROLLER_INSTANCE.GetLAxisVec();
	}
	else
	{
		// �R���g���[�����ڑ����A�L�[�{�[�h(WASD)�̓��͒l���擾
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W))
		{
			axisL.y += -1.0f;
		}
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S))
		{
			axisL.y += 1.0f;
		}
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A))
		{
			axisL.x += -1.0f;
		}
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D))
		{
			axisL.x += 1.0f;
		}
	}


	// �L�[���͂���A�j���[�V������Ԃֈڍs
	bool IsIdleStart = INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_W) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_D) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_S) &
		INPUT_INSTANCE.IsKeyOff(SDL_SCANCODE_A) &
		!(axisL.y != 0.0f) & !(axisL.x != 0.0f);

	// �ҋ@�A�j���J�n
	if (IsIdleStart)
	{
		if (m_animState == ANIM_LANDING)
		{
			if (!m_skelMeshComp->IsPlaying())
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_IDLE;
			}
		}
		else if ((m_animState != ANIM_IDLE))
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_IDLE], cAnimationSpeed * in_deltaTime);
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
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_RUNNING;
			}
		}

		//-----------------------------------------------------------------------------------+
		// ���s�A�j���[�V����
		//-----------------------------------------------------------------------------------+
		if ((axisL.y < -0.1f || axisL.y > 0.1f) && axisL.x < 0.1f && axisL.x > -0.1f)
		{
			// ����
			if (m_animState != ANIM_WALKING_FWD && axisL.y < -0.1f && axisL.y > -0.65f)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_WALKING_FWD], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_WALKING_FWD;
			}
			// RUN�A�j���J�n
			if (m_animState != ANIM_RUNNING && axisL.y <= -0.65f)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_RUNNING], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_RUNNING;
			}
		}
		
		// ���
		if (m_animState != ANIM_WALKING_BWD && axisL.y > 0.1f)
		{
			m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_WALKING_BWD], cAnimationSpeed * in_deltaTime);
			m_animState = ANIM_WALKING_BWD;
		}

		if ((axisL.x < -0.1f || axisL.x > 0.1f) && axisL.y <= 0.1f && axisL.y >= -0.1f)
		{
			// ����
			if (m_animState != ANIM_WALKING_LEFT && axisL.x < -0.1f)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_WALKING_LEFT], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_WALKING_LEFT;
			}
			// �E��
			if (m_animState != ANIM_WALKING_RIGHT && axisL.x > 0.1f)
			{
				m_skelMeshComp->PlayAnimation(m_animTypes[ANIM_WALKING_RIGHT], cAnimationSpeed * in_deltaTime);
				m_animState = ANIM_WALKING_RIGHT;
			}
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

// �v���C���[����̗L����
void PlayerHuman::SetActive(bool in_active)
{
	m_isActive = in_active;

	if (m_isActive)
	{
		delete m_cameraComp;
		m_cameraComp = new ThirdPersonCamera(this);
		m_cameraComp->SetAdjustForward(true);
		m_cameraComp->SetChaseOwnerForward(false);
		GAME_INSTANCE.SetCamera(m_cameraComp);
	}

}
