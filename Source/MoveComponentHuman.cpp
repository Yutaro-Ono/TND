#include "MoveComponentHuman.h"
#include "Math.h"
#include "InputController.h"
#include "Input.h"
#include "Mouse.h"
#include "PlayerHuman.h"
#include "PlayerManager.h"
#include "ThirdPersonCamera.h"

const float MoveComponentHuman::PLAYER_SPEED = 200.0f;

// �R���X�g���N�^
MoveComponentHuman::MoveComponentHuman(PlayerHuman* in_owner)
	:MoveComponent(in_owner)
	,m_playerHuman(in_owner)
{
	
}

// �f�X�g���N�^
MoveComponentHuman::~MoveComponentHuman()
{
}

// �X�V����
void MoveComponentHuman::Update(float in_deltaTime)
{
	if (m_isActive)
	{
		if (m_padIsActive)
		{
			MovementByController(in_deltaTime);
		}
		else
		{
			MovementByKeyboard(in_deltaTime);
		}
	}

	// �d�͏���
	m_owner->SetPosition(m_owner->GetPosition() - Vector3(0.0f, 0.0f, 2.0f));
}


// �v���C���[�̈ړ�����(�R���g���[��)
void MoveComponentHuman::MovementByController(float in_deltaTime)
{
	// ���X�e�B�b�N���͒l�̎擾
	Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();
	// �v���C���[�̑O�i�E�E�����x�N�g����` (�J�����)
	//Vector3 charaForwardVec = m_playerHuman->GetForward();
	Vector3 charaForwardVec = m_playerHuman->GetCamera()->GetForward();
	charaForwardVec.z = 0.0f;
	charaForwardVec.Normalize();
	Vector3 charaRightVec = Vector3::Cross(Vector3::UnitZ, charaForwardVec);

	// �J�����̎��������ɉ�]������
	if (axisL.x != 0.0f || axisL.y != 0.0f)
	{

		m_playerHuman->RotateToNewForward(charaForwardVec);
	}

	// �O�i�x�N�g���ƉE�����x�N�g������ړ��ʂ��Z�o
	Vector3 moveVec = Vector3::Zero;
	moveVec = charaForwardVec * -axisL.y + charaRightVec * axisL.x;

	// �v���C���[�̌��݈ʒu����A��`���������֑��x�������Z
	Vector3 resultPos = m_playerHuman->GetPosition();
	resultPos += moveVec * PLAYER_SPEED * in_deltaTime;

	// �ŏI�I�ȃv���C���[�̍��W���Z�b�g
	m_playerHuman->SetPosition(resultPos);
}

// �v���C���[�̈ړ�����(�L�[�{�[�h)
void MoveComponentHuman::MovementByKeyboard(float in_deltaTime)
{
	// �L�[���͒l
	Vector2 inputAxis = Vector2::Zero;


	// �v���C���[�̑O�i�E�E�����x�N�g����` (�J�����)
	//Vector3 charaForwardVec = m_playerHuman->GetForward();
	Vector3 charaForwardVec = m_playerHuman->GetCamera()->GetForward();
	charaForwardVec.z = 0.0f;
	charaForwardVec.Normalize();
	Vector3 charaRightVec = Vector3::Cross(Vector3::UnitZ, charaForwardVec);

	// �ړ��x�N�g��
	Vector3 moveVec = Vector3::Zero;

	// �L�[����WASD�ɂ��ړ�����
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W))
	{
		inputAxis.x += 1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S))
	{
		inputAxis.x += -1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A))
	{
		inputAxis.y += -1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D))
	{
		inputAxis.y += 1.0f;
	}

	// �J�����̎��������ɉ�]������
	if (inputAxis.x != 0.0f || inputAxis.y != 0.0f)
	{

		m_playerHuman->RotateToNewForward(charaForwardVec);
	}
	moveVec = charaForwardVec * inputAxis.x + charaRightVec * inputAxis.y;

	// �ŏI�I�ȍ��W���X�V���A�I�[�i�[�փZ�b�g
	Vector3 resultPos = m_playerHuman->GetPosition();
	resultPos += moveVec * PLAYER_SPEED * in_deltaTime;

	m_playerHuman->SetPosition(resultPos);
}
