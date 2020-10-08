#include "MoveComponentHuman.h"
#include "Math.h"
#include "InputController.h"
#include "Input.h"
#include "Mouse.h"
#include "PlayerHuman.h"
#include "PlayerManager.h"

const float MoveComponentHuman::PLAYER_SPEED = 200.0f;

// コンストラクタ
MoveComponentHuman::MoveComponentHuman(PlayerHuman* in_owner)
	:MoveComponent(in_owner)
	,m_playerHuman(in_owner)
{
	
}

// デストラクタ
MoveComponentHuman::~MoveComponentHuman()
{
}

// 更新処理
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


// プレイヤーの移動処理(コントローラ)
void MoveComponentHuman::MovementByController(float in_deltaTime)
{
	// 左スティック入力値の取得
	Vector2 axisL = CONTROLLER_INSTANCE.GetLAxisVec();
	// プレイヤーの前進・右方向ベクトル定義
	Vector3 charaForwardVec = m_playerHuman->GetForward();
	Vector3 charaRightVec = Vector3::Cross(Vector3::UnitZ, charaForwardVec);
	// 前進ベクトルと右方向ベクトルから移動量を算出
	Vector3 moveVec = Vector3::Zero;
	moveVec = charaForwardVec * -axisL.y + charaRightVec * axisL.x;

	// プレイヤーの現在位置から、定義した方向へ速度分を加算
	Vector3 resultPos = m_playerHuman->GetPosition();
	resultPos += moveVec * PLAYER_SPEED * in_deltaTime;

	// 最終的なプレイヤーの座標をセット
	m_playerHuman->SetPosition(resultPos);
}

// プレイヤーの移動処理(キーボード)
void MoveComponentHuman::MovementByKeyboard(float in_deltaTime)
{
	// プレイヤーの前進ベクトル
	Vector3 forwardVec = m_playerHuman->GetForward();
	// 移動ベクトル
	Vector3 moveVec = Vector3::Zero;

	// キー入力WASDによる移動処理
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


	// 最終的な座標を更新し、オーナーへセット
	Vector3 resultPos = m_playerHuman->GetPosition();
	resultPos += PLAYER_SPEED * in_deltaTime * forwardVec;

	m_playerHuman->SetPosition(resultPos);
}
