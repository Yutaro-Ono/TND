#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:

	MoveComponent(class Actor* in_owner, int in_updateOrder = 10);

	void Update(float in_deltaTime) override;

	// コントローラ・キーボードごとの入力処理
	virtual void MovementByController(float in_deltaTime) = 0;
	virtual void MovementByKeyboard(float in_deltaTime) = 0;

	float GetAngularSpeed() const { return m_angularSpeed; }
	float GetForwardSpeed() const { return m_forwardSpeed; }
	float GetStrafeSpeed() const { return m_strafeSpeed; }
	void SetAngularSpeed(float speed) { m_angularSpeed = speed; }
	void SetForwardSpeed(float speed) { m_forwardSpeed = speed; }
	void SetStrafeSpeed(float speed) { m_strafeSpeed = speed; }


	void SetActive(bool in_active) { m_isActive = in_active; }


protected:

	bool m_padIsActive;                // パッドがアクティブかどうか

	bool m_isActive;                   // MoveComponent自体がアクティブかどうか

	float m_angularSpeed;
	float m_forwardSpeed;
	float m_strafeSpeed;

};