#include "MoveComponentHuman.h"
#include "Math.h"
#include "InputController.h"
#include "Input.h"
//#include "InputMouse.h"
#include "Actor.h"

MoveComponentHuman::MoveComponentHuman(Actor* in_owner)
	:MoveComponent(in_owner)
{
	
}

MoveComponentHuman::~MoveComponentHuman()
{
}


// �v���C���[�̈ړ�����(�R���g���[��)
void MoveComponentHuman::MovementByController(float in_deltaTime)
{

}

// �v���C���[�̈ړ�����(�L�[�{�[�h)
void MoveComponentHuman::MovementByKeyboard(float in_deltaTime)
{

}
