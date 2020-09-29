#pragma once
#include "MoveComponent.h"

class MoveComponentHuman : public MoveComponent
{

public:

	MoveComponentHuman(class Actor* in_owner);
	~MoveComponentHuman();


	void Update(float in_deltaTime) override {}                          // コンポーネントの更新

	void ProcessInput(float in_deltaTime) override {}                                    // コンポーネントの入力処理

	void OnUpdateWorldTransform() override {}                      // ワールド座標変換用


	void MovementByController(float in_deltaTime) override;

	void MovementByKeyboard(float in_deltaTime) override;


protected:

	bool m_padIsActive;                 // コントローラがアクティブかどうか
};