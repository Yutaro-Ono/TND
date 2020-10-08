#pragma once
#include "MoveComponent.h"

class MoveComponentHuman : public MoveComponent
{

public:

	MoveComponentHuman(class PlayerHuman* in_owner);     // コンストラクタ
	~MoveComponentHuman();                               // デストラクタ


	void Update(float in_deltaTime) override;                      // コンポーネントの更新

	void ProcessInput(float in_deltaTime) override {}                                    // コンポーネントの入力処理

	void OnUpdateWorldTransform() override {}                      // ワールド座標変換用


	void MovementByController(float in_deltaTime) override;

	void MovementByKeyboard(float in_deltaTime) override;


protected:

	class PlayerHuman* m_playerHuman;       // プレイヤー(人間型)へのポインタ

	static const float PLAYER_SPEED;

};