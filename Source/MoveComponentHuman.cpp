#include "MoveComponentHuman.h"
#include "Math.h"
#include "InputController.h"
#include "Input.h"
#include "Mouse.h"
#include "PlayerHuman.h"
#include "PlayerManager.h"

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
}


// �v���C���[�̈ړ�����(�R���g���[��)
void MoveComponentHuman::MovementByController(float in_deltaTime)
{
	// ���X�e�B�b�N���͒l�̎擾
	Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();
	// �v���C���[�̑O�i�E�E�����x�N�g����`
	Vector3 charaForwardVec = m_playerHuman->GetForward();
	Vector3 charaRightVec = Vector3::Cross(Vector3::UnitZ, charaForwardVec);
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
	// �v���C���[�̑O�i�x�N�g��
	Vector3 forwardVec = m_playerHuman->GetForward();
	// �ړ��x�N�g��
	Vector3 moveVec = Vector3::Zero;

	// �L�[����WASD�ɂ��ړ�����
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_W))
	{
		moveVec.x = -1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_S))
	{
		moveVec.x = 1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_A))
	{
		moveVec.y = 1.0f;
	}
	if (INPUT_INSTANCE.IsKeyPressed(SDL_SCANCODE_D))
	{
		moveVec.y = -1.0f;
	}


	// �ŏI�I�ȍ��W���X�V���A�I�[�i�[�փZ�b�g
	Vector3 resultPos = m_playerHuman->GetPosition();
	resultPos += PLAYER_SPEED * in_deltaTime * forwardVec;

	m_playerHuman->SetPosition(resultPos);
}
