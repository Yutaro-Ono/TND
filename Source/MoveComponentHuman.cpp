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


// プレイヤーの移動処理(コントローラ)
void MoveComponentHuman::MovementByController(float in_deltaTime)
{

}

// プレイヤーの移動処理(キーボード)
void MoveComponentHuman::MovementByKeyboard(float in_deltaTime)
{

}
