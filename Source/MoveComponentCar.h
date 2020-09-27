#pragma once
#include "MoveComponent.h"

class MoveComponentCar : public MoveComponent
{

public:

	enum DRIVE_STATE
	{
		DRIVE_IDLE,
		DRIVE_ACCEL,
		DRIVE_BRAKE
	};

	MoveComponentCar(class Actor* in_owner);
	~MoveComponentCar();

	void Update(float in_deltaTime) override;                          // コンポーネントの更新

	void ProcessInput() override {}                                    // コンポーネントの入力処理

	void OnUpdateWorldTransform() override {}                      // ワールド座標変換用

	void MovementByController(float in_deltaTime);

	void MovementByKeyboard(float in_deltaTime);


	const float GetAccelValue() const { return m_accelValue; }
	void SetAccel(const float in_accel) { m_accelValue = in_accel; }

	void SetDriveState(DRIVE_STATE in_state) { m_driveState = in_state; }

protected:


	DRIVE_STATE m_driveState;

	bool m_padIsActive;                 // コントローラがアクティブかどうか

	// アクセル・ブレーキ加速度
	float m_accelValue;
	float m_brakeValue;
	// ハンドブレーキ
	float m_handBrake;
	// アクセル・ブレーキ限界値
	float m_accelLimit;
	float m_brakeLimit;

	static const float HAND_BRAKE_VALUE;                                                     // ハンドブレーキによる減速量
    static const float ACCEL_LIMIT;
	static const float BRAKE_LIMIT;
};