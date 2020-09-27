//-----------------------------------------------------------------------+
// �v���C���[�N���X
// 
// 
// copyright (C) 2019 Yutaro Ono. all rights reserved.
//-----------------------------------------------------------------------+
#include "Player.h"
#include "GameMain.h"
#include "Input.h"
#include "InputController.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "MoveComponentCar.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "PhysicsWorld.h"
#include "FollowCamera.h"
#include "FlameActor.h"
#include "SandSmoke.h"
#include "FlameSpark.h"
#include "AudioManager.h"


const int Player::CHAIN_LIMIT = 100;                         // �`�F�C���̃��~�b�^�[(99�܂�)
const int Player::RANK_MAX = 12;                             // �����N�̏��(12�܂�)
const float Player::CHAIN_TIMER = 15.0f;                     // �`�F�C���̃^�C�}�[
const float Player::HAND_BRAKE_VALUE = 30.0f;                 // �n���h�u���[�L���쎞�̃u���[�L�l
const float Player::ACCEL_LIMIT_MAX = 100.0f;                // �ő呬�x�̏���l(���݂̊�F150km/h)


// �R���X�g���N�^
Player::Player(bool in_isTitle)
	:m_animState(AnimState::IDLE)
	,m_moveState(MoveState::DRIVE_IDLE)
	,m_radian(Math::Pi / 180.0f)
	,m_accel(0.0f)
	,m_brake(0.0f)
	,m_handBrake(0.0f)
	,m_accelLimit(ACCEL_LIMIT_MAX)
	,m_brakeLimit(30.0f)
	,m_maxSpeedRank(0)
	,m_flameChain(0)
	,m_chainTimer(0.0f)
	,m_spinRadian(Math::Pi / 180.0f)
	,m_spinAngle(0.0f)
	,m_isJump(false)
	,m_onGround(true)
	,m_isTitle(in_isTitle)
{
	// ���b�V���̃��[�h
	// Mesh* mesh = RENDERER->GetMesh("Data/Meshes/FC/Actors/Player/Player.gpmesh");
	Mesh* mesh = GAME_INSTANCE.GetRenderer()->GetMesh("Data/Meshes/TND/Actors/Car/Player/Body/SM_suv_parts_LOD0_body_Internal.OBJ");
	//Mesh* mesh = RENDERER->GetMesh("Data/Meshes/TANK/tank.obj");
	m_meshComp = new MeshComponent(this);
	m_meshComp->SetMesh(mesh);

	if (m_isTitle == false)
	{
		MoveComponentCar* moveComp = new MoveComponentCar(this);
	}


	// �����蔻��Z�b�g
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

	// �Ƃ����΂̃Z�b�g
	SetTomosuFire();
	
	// �����̐���
	m_smoke = new SandSmoke(this);
	
	// �ΉԂ̐���
	m_spark = new FlameSpark(this);

	// ���ʉ�
	m_sound["Accel_Low"] = "Data/Music/SE/FC/Actors/Player/Accel/electric-fan-low1.wav";                      // �A�N�Z����
	m_sound["Accel_Medium"] = "Data/Music/SE/FC/Actors/Player/Accel/electric-fan-middle1.wav";
	m_sound["Accel_High"] = "Data/Music/SE/FC/Actors/Player/Accel/electric-fan-high1.wav";
	m_sound["Flame"] = "Data/Music/SE/FC/Actors/Player/Flame/se_maoudamashii_element_fire12_.wav";            // �Ƃ����΂̔R���オ����ʉ�
	m_sound["HitWall"] = "Data/Music/SE/FC/Actors/Player/Collision/punch-real1.wav";                          // �ǂɏՓˎ�
	m_sound["Add_Chain"] = "Data/Music/SE/FC/System/Chain/decision16_.wav";


	// ���ʉ��t�@�C���̎擾
	AUDIO->GetSound(m_sound["Accel_Low"]);
	AUDIO->GetSound(m_sound["Accel_Medium"]);
	AUDIO->GetSound(m_sound["Accel_High"]);
	AUDIO->GetSound(m_sound["Flame"]);
	AUDIO->GetSound(m_sound["HitWall"]);
	AUDIO->GetSound(m_sound["HitFlame"]);
	AUDIO->GetSound(m_sound["Add_Chain"]);

	printf("PlayerActor�쐬 id:[%5d] this : (0x%p)\n", m_ID, this);
	m_jumpVec = Vector3(0, 0, 0);
}


// �f�X�g���N�^
Player::~Player()
{
	delete m_hitBox;
	delete m_hitGroundBox;
}


// �X�V����
void Player::UpdateActor(float in_deltaTime)
{
	// �X�s�[�h�����N�̍X�V
	// UpdateSpeedRank();

	// �^�C�g���V�[��
	if (m_isTitle == true)
	{
		// �^�C�g����ʂł̉�]����
		SpinTitle(in_deltaTime);
	}
	else
	{
		// �Q�[���V�[�����̓v���C���[�̑���
		// UpdateMovement(in_deltaTime);
	}

	// �Ƃ����Ώ���
	for (int i = 0; i < m_maxSpeedRank; i++)
	{
		if (m_tomosuFire[i] == nullptr)
		{
			m_tomosuFire[i] = new FlameActor();

			m_tomosuFire[i]->SetFlameLife(0.06f);
			m_tomosuFire[i]->SetScale(30.0f);
		}

		float rad = -(i + 1) / m_tomosuFire.size() * Math::TwoPi;

		Vector3 firePos = Vector3(50.0f * Math::Cos(rad * (i + 2)), -1.0f, 50.0f * Math::Sin(rad * (i + 2)));

		if ((i + 1) % 2)
		{
			firePos.y -= 13.0f;
		}
		else
		{
			firePos.y += 15.0f;
		}

		// �v���C���[�̃��[���h�s����Ƃ����΂̍��W�ɕϊ�
		firePos = Vector3::Transform(firePos, GetWorldTransform());

		m_tomosuFire[i]->SetPosition(firePos);


		// printf("x : %f, y : %f, z : %f\n", m_tomosuFire[i]->GetPosition().x, m_tomosuFire[i]->GetPosition().y, m_tomosuFire[i]->GetPosition().z);

	}

	// SE�Đ�
	if (m_isTitle == false)
	{
		if (m_maxSpeedRank < 6)
		{
			if ((m_accel >= 5.0f || m_brake >= 3.0f) && AUDIO->IsPlayingSound(m_sound["Accel_Low"]) == false)
			{
				AUDIO->PlaySound(m_sound["Accel_Low"]);
			}
			else if (m_accel < 5.0f && m_brake < 3.0f)
			{
				AUDIO->StopSound(m_sound["Accel_Low"]);
			}
		}


		if (m_maxSpeedRank > 5)
		{
			if ((m_accel >= 5.0f || m_brake >= 3.0f) && AUDIO->IsPlayingSound(m_sound["Accel_Medium"]) == false)
			{
				AUDIO->PlaySound(m_sound["Accel_Medium"]);
			}
			else if (m_accel < 5.0f && m_brake < 3.0f)
			{
				AUDIO->StopSound(m_sound["Accel_Medium"]);
			}

			if (AUDIO->IsPlayingSound(m_sound["Accel_Low"]) == true)
			{
				AUDIO->StopSound(m_sound["Accel_Low"]);
			}
		}

		if (m_maxSpeedRank > 10)
		{
			if ((m_accel >= 5.0f || m_brake >= 3.0f) && AUDIO->IsPlayingSound(m_sound["Accel_High"]) == false)
			{
				AUDIO->PlaySound(m_sound["Accel_High"]);
			}
			else if (m_accel < 5.0f && m_brake < 3.0f)
			{
				AUDIO->StopSound(m_sound["Accel_High"]);
			}

			if (AUDIO->IsPlayingSound(m_sound["Accel_Medium"]) == true)
			{
				AUDIO->StopSound(m_sound["Accel_Medium"]);
			}
		}


		// ��SE�Đ�
		if (m_maxSpeedRank >= 1 && AUDIO->IsPlayingSound(m_sound["Flame"]) == false)
		{
			AUDIO->PlaySound(m_sound["Flame"]);
		}
	}
	else
	{
		if (AUDIO->IsPlayingSound(m_sound["Accel_Low"]) == false)
		{
			AUDIO->PlaySound(m_sound["Accel_Low"]);
		}

		if (AUDIO->IsPlayingSound(m_sound["Flame"]) == false)
		{
			AUDIO->PlaySound(m_sound["Flame"]);
		}
	}

	// �`�F�C���^�C�}�[�X�V
	if (m_chainTimer > 0.0f && m_flameChain >= 1)
	{
		m_chainTimer -= in_deltaTime;
	}

	// �^�C�}�[��0�ɂȂ����Ƃ��A�`�F�C���������炷
	if (m_chainTimer <= 0.0f && m_flameChain >= 1)
	{
		m_flameChain--;
		m_chainTimer = CHAIN_TIMER;
	}


	// �����蔻�菈���Œn�ʃt���O��false�ɂ��A���̌�̓����蔻�菈����true�ɂȂ邩�`�F�b�N����
	m_onGround = false;



}


// ���[���h�ϊ��s����v�Z
void Player::ComputeWorldTransform2()
{

	// �X�P�[�����O����]�����s�ړ��ƂȂ�悤�ɕϊ��s����쐬
	m_worldTransform = Matrix4::CreateScale(m_scale);

	// Z����]
	m_worldTransform *= Matrix4::CreateRotationZ(m_radian);

	// �|�W�V����
	m_worldTransform *= Matrix4::CreateTranslation(m_position);


	// �A�N�^�[�������Ă���S�R���|�[�l���g�̕ϊ����s��
	for (auto comp : m_components)
	{
		comp->OnUpdateWorldTransform();
	}

}

//-----------------------------------------------------------------------------+
// �`�F�C���֘A����
//-----------------------------------------------------------------------------+
// �`�F�C�����̉��Z
void Player::AddFlameChain()
{
	// �`�F�C���̃��~�b�g��艺�̎��`�F�C���X�V
	if (m_flameChain < CHAIN_LIMIT)
	{
		m_flameChain++;
	}

	// SE�Đ�
	if (m_flameChain >= 2)
	{
		AUDIO->PlaySound(m_sound["Add_Chain"]);
	}


	// �^�C�}�[��ݒ�
	m_chainTimer = CHAIN_TIMER;
}

// �`�F�C�����̌��Z
void Player::SubFlameChain()
{
	if (m_flameChain != 0)
	{
		m_flameChain--;
	}

}


// �S�ẴT�E���h���X�g�b�v
void Player::AllStopSound()
{
	// Sound���~�߂�
	if (AUDIO->IsPlayingSound(m_sound["Accel_Low"]))
	{
		AUDIO->StopSound(m_sound["Accel_Low"]);
	}
	if (AUDIO->IsPlayingSound(m_sound["Accel_Medium"]))
	{
		AUDIO->StopSound(m_sound["Accel_Medium"]);
	}
	if (AUDIO->IsPlayingSound(m_sound["Accel_High"]))
	{
		AUDIO->StopSound(m_sound["Accel_High"]);
	}

	if (AUDIO->IsPlayingSound(m_sound["Flame"]))
	{
		AUDIO->StopSound(m_sound["Flame"]);
	}
}


// �v���C���[�̃X�e�[�^�X������(��Ƀt���[���[�h�Ŏ擾�������̏�����)
void Player::InitStatus()
{
	m_accel = 0.0f;
	m_brake = 0.0f;

	m_accelLimit = 10.0f;
	m_brakeLimit = 5.0f;

	for (int i = 0; i < RANK_MAX; i++)
	{
		m_tomosuFire[i]->SetState(STATE_DEAD);
		m_tomosuFire[i] = nullptr;
	}

	m_maxSpeedRank = 0;
	m_flameChain = 0;
	m_chainTimer = 0.0f;
}


// �Ƃ����΂̃Z�b�g
void Player::SetTomosuFire()
{
	for (int i = 0; i < 12; i++)
	{
		m_tomosuFire.push_back(new FlameActor());

		float rad = (i + 1) / m_tomosuFire.size() * Math::Pi * 2 + m_spinRadian;

		Vector3 firePos = Vector3(50.0f * Math::Cos(rad * (i + 1)), 0.0f, 50.0f * Math::Sin(rad * (i + 1)));


		if (i % 2 == 0)
		{
			firePos.y = -5.0f;
		}
		else
		{
			firePos.y = +5.0f;
		}

		firePos = Vector3::Transform(firePos, GetWorldTransform());

		m_tomosuFire[i]->SetPosition(firePos);

		m_tomosuFire[i]->SetFlameLife(0.06f);
		m_tomosuFire[i]->SetScale(30.0f);
	}
}


// �^�C�g���p�v���C���[
void Player::SetTitlePlayer()
{
	m_accelLimit = ACCEL_LIMIT_MAX;
}


// �Փˎ��̉����o������
void Player::CollisionFix(BoxCollider * in_hitPlayerBox, BoxCollider * in_hitBox)
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

	// �ǏՓˎ��̏���
	if (fix.x > 5.0f || fix.x < -5.0f || fix.y > 5.0f || fix.y < -5.0f)
	{
		// ���ʉ��Đ�
		AUDIO->PlaySound(m_sound["HitWall"]);

		// �`�F�C���̏I��
		// RemoveFlameChain();

		// �A�N�Z������
		m_accel -= m_accel / 2.0f;
	}

	// �W�����v�A�N�V�������ɓ����ǂɓ��������Ƃ�
	if (in_hitPlayerBox == m_hitHeadBox && m_isJump)
	{
		//�W�����v������ɍs���������t�]������
		m_jumpVec = Vector3(0, 0, -1.0f);
		m_position += m_jumpVec; // ��������ɓ�����Ȃ��l�ɗ���
	}

	// �ʒu���ς�����̂Ń{�b�N�X�Čv�Z
	m_hitBox->OnUpdateWorldTransform();

	// printf("[%f, %f, %f]\n", m_position.x, m_position.y, m_position.z);
}

void Player::UpdateMovement(float in_deltaTime)
{
	//�L��������
	const float speed = 8.0f;
	Vector3 forwardVec = Vector3(60.0f, 0.0f, 0.0f);
	Vector3 rightVec = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 charaForwardVec = GetForward();

	// �L�����N�^�[�ړ�
	Vector3 DirVec(0.0f, 0.0f, 0.0f);

	// �A�N�Z�����E�l
	if (m_accel > ACCEL_LIMIT_MAX)
	{
		m_accel = ACCEL_LIMIT_MAX;
	}

	//---------------------------------------------------------------------------------------------+
	// �v���C���[�̑���
	//---------------------------------------------------------------------------------------------+
	// �R���g���[���[�ڑ����Ɩ��ڑ����ŏ����𕪂���
	if (CONTROLLER_INSTANCE.IsAvailable() && m_isTitle == false)
	{
		// �E�g���K�[�A���g���K�[�擾
		float triggerR = CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		float triggerL = CONTROLLER_INSTANCE.GetAxisValue(SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		// ���X�e�B�b�N���͒l���擾
		Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();

		// �g���K�[���͒l��\��
		// printf("R_TRIGGER : %f | L_TRIGGER : %f \n", triggerR, triggerL);

		//------------------------------------------------------------------------------+
		// �O�i
		//------------------------------------------------------------------------------+
		if (triggerR != 0.0f)
		{
			// �A�N�Z����Ԃɂ���
			SetMoveState(MoveState::DRIVE_ACCEL);

			// �A�N�Z���̍ő�l�������Ă��Ȃ���΍X�V���A�����Ă�����ő�l�𐳋K��
			if (m_accel < m_accelLimit)
			{
				m_accel += speed * (triggerR * in_deltaTime);
			}
			else
			{
				m_accel = m_accelLimit;
			}
		}
		else if (m_accel != 0.0f)       // �A�N�Z�����|�����Ă��鎞���X�V
		{
			m_accel -= (m_accel / 1.5f) * in_deltaTime;
		}

		//---------------------------------------------------------------------------------+
		// ���
		//---------------------------------------------------------------------------------+
		if (triggerL != 0.0f)
		{
			// �ړ���Ԃ��u���[�L�ɂ���
			SetMoveState(MoveState::DRIVE_BRAKE);

			// �u���[�L�̍ő�l�������Ă��Ȃ���΍X�V���A�����Ă���΍ő�l�𐳋K��
			if (m_brake < m_brakeLimit)
			{
				m_brake += speed * (triggerL * in_deltaTime);
			}
			else
			{
				m_brake = m_brakeLimit;
			}
		}
		else if (m_brake != 0.0f)
		{
			m_brake -= (m_brake / 1.5f) * in_deltaTime;
		}

		//-----------------------------------------------------------------------------------+
		// �����x��0�ɖ߂�����
		//-----------------------------------------------------------------------------------+
		// �A�N�Z���E�u���[�L�����l�ȉ��Ȃ�0�ɖ߂�
		if (m_accel < 0.2f)
		{
			m_accel = 0.0f;
		}
		if (m_brake < 0.2f)
		{
			m_brake = 0.0f;
		}
		// �A�N�Z���E�u���[�L�����Ƃ�0�Ȃ�IDLE��Ԃ�
		if (m_accel == 0.0f && m_brake == 0.0f)
		{
			SetMoveState(MoveState::DRIVE_IDLE);
		}

		//----------------------------------------------------------------------------------+
		// �A�N�Z���E�u���[�L�̑��E����
		//----------------------------------------------------------------------------------+
		// �A�N�Z���̒l�����ȉ����A�N�Z���ƃu���[�L�������͂ő��E
		if (triggerR == 1.0f && triggerL == 1.0f)
		{
			if (m_accel >= m_brake)
			{
				m_accel -= m_accel / 10.0f;
			}
			else
			{
				SetMoveState(MoveState::DRIVE_IDLE);
				m_accel = m_brake;
			}
		}

		// �A�N�Z���E�o�b�N�̑��x��\�� (�f�o�b�O)
		// printf("ACCEL_SPEED : %f | BACK_SPEED : %f \n", m_accel, m_brake);

		// �n���h�u���[�L����(�R���g���[���[A�{�^�������Ŕ���)
		if (CONTROLLER_INSTANCE.IsPressed(SDL_CONTROLLER_BUTTON_A) && m_accel > 5.0f)
		{
			m_handBrake = HAND_BRAKE_VALUE;
			m_accel -= m_handBrake;
		}
		else
		{
			m_handBrake = 0;
		}

		// �O�i�l���X�V
		DirVec.x += forwardVec.x * (m_accel + -m_brake) * in_deltaTime;

		// ���E�̉�]�A�ړ�(�A�N�Z�����O��ǂ��炩�ɂ������Ă��鎞)
		if (m_accel != 0.0f || m_brake != 0.0f)
		{
			// �A�N�Z����
			if (m_moveState == MoveState::DRIVE_ACCEL)
			{
				// ���E��]
				DirVec.y += rightVec.y * axisL.x * (m_accel / m_accelLimit) * (m_accelLimit / 5.0f) * in_deltaTime;
				// Z����]
				m_radian += axisL.x * (m_accel / m_accelLimit) * (m_accelLimit / 60.0f) * in_deltaTime;
			}
			// �o�b�N��
			if (m_moveState == MoveState::DRIVE_BRAKE)
			{
				// ���E��]
				DirVec.y += rightVec.y * axisL.x * (m_brake / m_brakeLimit) * 0.5f * in_deltaTime;
				// Z����]
				m_radian += axisL.x * (m_brake / m_brakeLimit) * 0.5f * in_deltaTime;
			}

			// ���E��
			if (m_accel >= 1.0f && m_brake >= 1.0f)
			{
				// ���E��]
				DirVec.y += rightVec.y * axisL.x * 0.5f * in_deltaTime;
				// Z����]
				m_radian += axisL.x * 0.5f * in_deltaTime;
			}

		}


		// Y���̉�]�l
		float spinValue = 6.5f * (m_accel / ACCEL_LIMIT_MAX) * in_deltaTime;

		// Y����]
		// �A�N�Z���ƃu���[�L�������ɂ������Ă��Ȃ��ꍇ�̂�
		if (m_moveState != MoveState::DRIVE_IDLE)
		{
			// �A�N�Z�����͑O���ɉ�]
			if (m_moveState == MoveState::DRIVE_ACCEL)
			{
				// m_spinRadian += 6.5f * (m_accel / m_accelLimit) * in_deltaTime;
				m_spinRadian += spinValue;
			}

			// �u���[�L���͌���ɉ�]
			if (m_moveState == MoveState::DRIVE_BRAKE)
			{
				m_spinRadian -= (m_brake / m_brakeLimit) * in_deltaTime;
			}
		}

	}
	//------------------------------------------------------------------------------------------------+
	// �L�[�{�[�h����
	//------------------------------------------------------------------------------------------------+
	else if (m_isTitle == false)
	{
		// �E�g���K�[�A���g���K�[�擾
		float triggerR = 0.0f;
		float triggerL = 0.0f;
		// ���X�e�B�b�N���͒l���擾
		float vertAxis = 0.0f;

		// �L�[���͎擾(�A�N�Z��)
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_UP))
		{
			triggerR = 1.0f;
		}
		else
		{
			triggerR = 0.0f;
		}

		// �L�[���͎擾(�o�b�N)
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_DOWN))
		{
			triggerL = 1.0f;
		}
		else
		{
			triggerL = 0.0f;
		}

		// �L�[���͎擾(���E�ړ�)
		if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_LEFT))
		{
			vertAxis = -1.0f;
		}
		else if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D) || INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_RIGHT))
		{
			vertAxis = 1.0f;
		}
		else
		{
			vertAxis = 0.0f;
		}

		// �g���K�[���͒l��\��
		// printf("R_TRIGGER : %f | L_TRIGGER : %f \n", triggerR, triggerL);

		//------------------------------------------------------------------------------+
		// �O�i
		//------------------------------------------------------------------------------+
		if (triggerR != 0.0f)
		{
			// �A�N�Z����Ԃɂ���
			SetMoveState(MoveState::DRIVE_ACCEL);

			// �A�N�Z���̍ő�l�������Ă��Ȃ���΍X�V���A�����Ă�����ő�l�𐳋K��
			if (m_accel < m_accelLimit)
			{
				m_accel += speed * (triggerR * in_deltaTime);

			}
			else
			{
				m_accel = m_accelLimit;
			}
		}
		else if (m_accel != 0.0f)       // �A�N�Z�����|�����Ă��鎞���X�V
		{
			m_accel -= (m_accel / 1.5f) * in_deltaTime;
		}

		//---------------------------------------------------------------------------------+
		// ���
		//---------------------------------------------------------------------------------+
		if (triggerL != 0.0f)
		{
			// �ړ���Ԃ��u���[�L�ɂ���
			SetMoveState(MoveState::DRIVE_BRAKE);

			// �u���[�L�̍ő�l�������Ă��Ȃ���΍X�V���A�����Ă���΍ő�l�𐳋K��
			if (m_brake < m_brakeLimit)
			{
				m_brake += 8.0f * (triggerL * in_deltaTime);
			}
			else
			{
				m_brake = m_brakeLimit;
			}

		}
		else if (m_brake != 0.0f)
		{
			m_brake -= (m_brake / 1.5f) * in_deltaTime;
		}


		//-----------------------------------------------------------------------------------+
		// �����x��0�ɖ߂�����
		//-----------------------------------------------------------------------------------+
		// �A�N�Z���E�u���[�L�����l�ȉ��Ȃ�0�ɖ߂�
		if (m_accel < 0.2f)
		{
			m_accel = 0.0f;
		}
		if (m_brake < 0.2f)
		{
			m_brake = 0.0f;
		}
		// �A�N�Z���E�u���[�L�����Ƃ�0�Ȃ�IDLE��Ԃ�
		if (m_accel == 0.0f && m_brake == 0.0f)
		{
			SetMoveState(MoveState::DRIVE_IDLE);
		}

		//----------------------------------------------------------------------------------+
		// �A�N�Z���E�u���[�L�̑��E����
		//----------------------------------------------------------------------------------+
		// �A�N�Z���̒l�����ȉ����A�N�Z���ƃu���[�L�������͂ő��E
		if (triggerR == 1.0f && triggerL == 1.0f)
		{
			if (m_accel >= m_brake)
			{
				m_accel -= m_accel / 10.0f;
			}
			else
			{
				SetMoveState(MoveState::DRIVE_IDLE);
				m_accel = m_brake;
			}
		}

		// �A�N�Z���E�o�b�N�̑��x��\�� (�f�o�b�O)
		// printf("ACCEL_SPEED : %f | BACK_SPEED : %f \n", m_accel, m_brake);

		// �O�i�l���X�V
		DirVec.x += forwardVec.x * (m_accel + -m_brake) * in_deltaTime;


		// ���E�̉�]�A�ړ�(�A�N�Z�����O��ǂ��炩�ɂ������Ă��鎞)
		if (m_accel != 0.0f || m_brake != 0.0f)
		{
			// �A�N�Z����
			if (m_moveState == MoveState::DRIVE_ACCEL)
			{
				// ���E��]
				DirVec.y += rightVec.y * vertAxis * (m_accel / m_accelLimit) * (m_accelLimit / 5.0f) * in_deltaTime;
				// Z����]
				m_radian += vertAxis * (m_accel / m_accelLimit) * (m_accelLimit / 60.0f) * in_deltaTime;
			}
			// �o�b�N��
			if (m_moveState == MoveState::DRIVE_BRAKE)
			{
				// ���E��]
				DirVec.y += rightVec.y * vertAxis * (m_brake / m_brakeLimit) * 0.5f * in_deltaTime;
				// Z����]
				m_radian += vertAxis * (m_brake / m_brakeLimit) * 0.5f * in_deltaTime;
			}

			// ���E��
			if (m_accel >= 1.0f && m_brake >= 1.0f)
			{
				// ���E��]
				DirVec.y += rightVec.y * vertAxis * 0.5f * in_deltaTime;
				// Z����]
				m_radian += vertAxis * 0.5f * in_deltaTime;
			}

		}

		// Y���̉�]�l
		float spinValue = 6.5f * (m_accel / ACCEL_LIMIT_MAX) * in_deltaTime;

		// Y����]
		// �A�N�Z���ƃu���[�L�������ɂ������Ă��Ȃ��ꍇ�̂�
		if (m_moveState != MoveState::DRIVE_IDLE)
		{
			// �A�N�Z�����͑O���ɉ�]
			if (m_moveState == MoveState::DRIVE_ACCEL)
			{
				// m_spinRadian += 6.5f * (m_accel / m_accelLimit) * in_deltaTime;
				m_spinRadian += spinValue;
			}

			// �u���[�L���͌���ɉ�]
			if (m_moveState == MoveState::DRIVE_BRAKE)
			{
				m_spinRadian -= (m_brake / m_brakeLimit) * in_deltaTime;
			}
		}

	}

	// �O�i�܂��͌�ގ��̂ݑO�i�x�N�g�����X�V
	if (DirVec.x > 0.5f || DirVec.x < -0.5f)
	{
		// �����L�[���͂̒l��O�i�x�N�g���ɃZ�b�g
		charaForwardVec = DirVec;

		// ��Ԃ�"�X�s��"��
		m_animState = AnimState::SPIN;

		// ���݂̃X�s�[�h��ۑ�
		m_speed = speed * in_deltaTime;

	}
	else
	{
		// ��Ԃ�"Idle"��
		m_animState = AnimState::IDLE;

		charaForwardVec = Vector3(0, 0, 0);
	}

	// Y��Rotation��]
	// m_rotation = Quaternion::Quaternion(Vector3::UnitY, m_spinRadian);
	// Z��Rotation��]
	m_rotation = Quaternion::Quaternion(Vector3::UnitZ, m_radian);

	// Rotation����O�i�x�N�g�����X�V���A���ۂ̃|�W�V�����ɑ���
	charaForwardVec = Vector3::Transform(charaForwardVec, m_rotation);
	m_position += m_speed * charaForwardVec - Vector3(0.0f, 0.0f, 0.0f);

	// ���[���h���W�ϊ�
	ComputeWorldTransform2();

}


// �X�s�[�h�����N�̍X�V(�S5�i�K)
void Player::UpdateSpeedRank()
{

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.132f)
	{
		m_maxSpeedRank = 1;
	}

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.2f)
	{
		m_maxSpeedRank = 2;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.3f)
	{
		m_maxSpeedRank = 3;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.4f)
	{
		m_maxSpeedRank = 4;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.5f)
	{
		m_maxSpeedRank = 5;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.6f)
	{
		m_maxSpeedRank = 6;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.65f)
	{
		m_maxSpeedRank = 7;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.7f)
	{
		m_maxSpeedRank = 8;
	}
	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.75f)
	{
		m_maxSpeedRank = 9;
	}

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.85f)
	{
		m_maxSpeedRank = 10;
	}

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.90f)
	{
		m_maxSpeedRank = 11;
	}

	if (m_accelLimit >= ACCEL_LIMIT_MAX * 0.98f)
	{
		m_maxSpeedRank = 12;
	}

}

void Player::SpinTitle(float in_deltaTime)
{
	m_spinRadian += 1.0f * in_deltaTime;
	// Y��Rotation��]
	m_rotation.y = m_spinRadian;

	ComputeWorldTransform2();

}
